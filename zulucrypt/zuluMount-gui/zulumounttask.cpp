/*
 *
 *  Copyright (c) 2014-2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bin_path.h"
#include "zulumounttask.h"

#include <QDir>
#include <QString>
#include <QDebug>
#include <QFile>

#include <stdlib.h>

static QString _convert_lvm_path( const QString& dev )
{
	auto volume = dev ;
	auto e = dev.toLatin1() ;

	char * begin = e.data() ;
	char * end = begin + e.size() ;

	char * k ;
	char * c ;
	char * d ;

	for( auto it = begin + 3 ; it < end ; it++ ){

		k = it - 2 ;
		c = it - 1 ;
		d = it ;

		if( *k != '-' && *c == '-' && *d != '-' ){

			*c = '/' ;

			volume = e ;

			while( volume.contains( "--" ) ){

				volume.replace( "--","-" ) ;
			}

			break ;
		}
	}

	return volume ;
}

static QString _convert_md_raid_path( const QString& dev,bool wait )
{
	auto volume = dev ;

	decltype( volume ) dev_1 ;

	QDir f ;

	if( wait ){
		/*
		 * wait for a while because things dont always happen as expected if we check too soon.
		 */
		utility::Task::wait( 4 ) ;
	}

	if( utility::pathExists( "/dev/md/" ) ){

		auto l = utility::directoryList( "/dev/md/" ) ;

		for( const auto& it : l ){

			dev_1 = "/dev/md/" + it ;
			f.setPath( dev_1 ) ;

			if( f.canonicalPath() == dev ){

				volume = dev_1 ;

				break ;
			}
		}
	}

	return volume ;
}

static QString _device( const QString& device )
{
	if( device.startsWith( "UUID" ) ){

		return device ;
	}else{
		auto d = device ;
		return d.replace( "\"","\"\"\"" ) ;
	}
}

static bool _volumeIsSystemVolume( const QString& e )
{
	return utility::Task( utility::appendUserUID( "%1 -S" ).arg( zuluMountPath ) ).splitOutput( '\n' ).contains( e ) ;
}

QStringList zuluMountTask::mountedVolumeList( void )
{
	return utility::Task( utility::appendUserUID( "%1 -E" ).arg( zuluMountPath ) ).splitOutput( '\n' ) ;
}

volumeEntryProperties _getVolumeProperties( const QString& e )
{
	auto device = _device( e ) ;

	auto r = utility::Task( utility::appendUserUID( "%1 -L -d \"%2\"" ).arg( zuluMountPath,device ) ) ;

	if( r.success() ) {

		return volumeEntryProperties( r.splitOutput( '\t' ),_volumeIsSystemVolume( device ) ) ;
	}else{
		return volumeEntryProperties() ;
	}
}

Task::future< volumeEntryProperties >& zuluMountTask::getVolumeProperties( const QString& e )
{
	return Task::run< volumeEntryProperties >( [ e ](){ return _getVolumeProperties( e ) ; } ) ;
}

Task::future< QString >& zuluMountTask::volumeProperties( const QString& v,const QString& volumeType )
{
	return Task::run< QString >( [ = ](){

		if( v.isEmpty() ){

			return QString() ;
		}

		auto volume = _device( v ) ;

		auto r = utility::Task( utility::appendUserUID( "%1 -s -d \"%2\"" ).arg( zuluMountPath,volume ) ) ;

		if( r.ok() ){

			return QString( r.output() ) ;
		}else{
			if( volumeType.contains( "crypto_PLAIN\n" ) ){

				/*
				* this could be a plain volume opened with an offset
				*/

				auto e = utility::appendUserUID( "%1 -s -o bogusNecessaryArgument -d \"%2\"" ) ;
				r = utility::Task( e.arg( zuluMountPath,volume ) ) ;

				if( r.ok() ){

					return QString( r.output() ) ;
				}else{
					return QString() ;
				}
			}else{
				return QString() ;
			}
		}
	} ) ;
}

utility::Task zuluMountTask::volumeUnmount( const QString& volumePath,const QString& volumeType,const QString& powerOffCommand )
{
	auto _run = []( const QString& exe ){

		auto e = utility::Task( exe ) ;

		QString output = e.output() ;
		int index = output.indexOf( QChar( ':' ) ) ;
		e.output( output.mid( index + 1 ).toLatin1() ) ;

		return e ;
	} ;

	auto volume = _device( volumePath ) ;

	auto r = _run( utility::appendUserUID( "%1 -u -d \"%2\"" ).arg( zuluMountPath,volume ) ) ;

	if( r.failed() ){

		if( volumeType.contains( "crypto_PLAIN\n" ) ){
			/*
			 * we could be trying to unmount a volume with an offset
			 */
			r = _run( utility::appendUserUID( "%1 -o bogusNecessaryArgument -u -d \"%2\"" ).arg( zuluMountPath,volume ) ) ;
		}
	}

	if( r.success() && !powerOffCommand.isEmpty() ){

		auto v = volumePath ;

		if( v.startsWith( "/dev/sd" ) || v.startsWith( "/dev/hd" ) ){

			for( char i = '0' ; i < '9' ; i++ ){

				v.remove( i ) ;
			}
		}

		utility::Task( powerOffCommand.arg( v ) ) ;
	}

	return r ;
}

Task::future< utility::Task >& zuluMountTask::unmountVolume( const QString& volumePath,const QString& volumeType,const QString& powerOffCommand )
{
	return Task::run< utility::Task >( [ = ](){ return zuluMountTask::volumeUnmount( volumePath,volumeType,powerOffCommand ) ; } ) ;
}

static QString _excludeVolumePath()
{
	return utility::homePath() + "/.zuluCrypt/zuluMount-gui-excludeVolumes" ;
}

struct deviceList
{
	QString deviceName ;
	QString uniqueName ;
};

static QVector< deviceList > _getDevices()
{
	auto p = "/dev/disk/by-id/" ;

	auto l = utility::directoryList( p ) ;

	decltype( _getDevices() ) devices ;

	if( l.isEmpty() ){

		return devices ;
	}else{
		auto _not_present = [ &devices ]( const QString& e ){

			for( const auto& it : devices ){

				if( it.deviceName == e ){

					return false ;
				}
			}

			return true ;
		} ;

		auto _device_path = []( const QString& dev ){

			if( dev.startsWith( "/dev/dm-" ) ){

				QFile file( "/sys/block/" + dev.split( '/' ).last() + "/dm/name" ) ;

				if( file.open( QIODevice::ReadOnly ) ){

					QString e = file.readAll() ;
					e.truncate( e.size() - 1 ) ;
					return _convert_lvm_path( "/dev/" + e ) ;
				}else{
					return dev ;
				}

			}else if( dev.startsWith( "/dev/md" ) ){

				return _convert_md_raid_path( dev,false ) ;
			}else{
				return dev ;
			}
		} ;

		QDir e ;

		for( const auto& it : l ){

			e.setPath( p + it ) ;

			if( !it.startsWith( "edd-int13_dev" ) ){

				const auto& q = _device_path( e.canonicalPath() ) ;

				if( _not_present( q ) ){

					devices.append( { q,it } ) ;
				}
			}
		}
	}

	return devices ;
}

static QString _getUniqueName( const QString& device )
{
	for( const auto& it : _getDevices() ){

		if( it.deviceName == device ){

			return it.uniqueName ;
		}
	}

	return QString() ;
}

static QStringList _split( QFile& f )
{
	return utility::split( f.readAll() ) ;
}

void zuluMountTask::addVolumeToHiddenVolumeList( const QString& e )
{
	QFile f( _excludeVolumePath() ) ;

	if( f.open( QIODevice::WriteOnly | QIODevice::Append ) ){

		auto a = _getUniqueName( e ) ;

		if( !a.isEmpty() ){

			f.write( a.toLatin1() + "\n" ) ;
		}
	}
}

QStringList zuluMountTask::hiddenVolumeList()
{
	QFile f( _excludeVolumePath() ) ;

	if( f.open( QIODevice::ReadOnly ) ){

		auto l = _getDevices() ;

		decltype( _split( f ) ) e ;

		auto g = _split( f ) ;

		for( const auto& it : l ){

			if( g.contains( it.uniqueName ) ){

				e.append( it.deviceName ) ;
			}
		}

		return e ;
	}else{
		return QStringList() ;
	}
}

void zuluMountTask::removeVolumeFromHiddenVolumeList( const QString& e )
{
	auto _get_hidden_volume_list = [](){

		QFile f( _excludeVolumePath() ) ;

		if( f.open( QIODevice::ReadOnly ) ){

			return _split( f ) ;
		}else{
			return QStringList() ;
		}
	} ;

	auto _remove_entry = []( QStringList l,const QString& e ){

		if( !l.isEmpty() ){

			l.removeAll( _getUniqueName( e ) ) ;
		}

		return l ;
	} ;

	auto _update_list = []( const QStringList& l ){

		if( !l.isEmpty() ){

			QFile f( _excludeVolumePath() ) ;

			if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ){

				for( const auto& it : l ){

					f.write( it.toLatin1() + "\n" ) ;
				}
			}
		}
	} ;

	_update_list( _remove_entry( _get_hidden_volume_list(),e ) ) ;
}

Task::future< QVector< volumeEntryProperties > >& zuluMountTask::updateVolumeList()
{
	return Task::run< QVector< volumeEntryProperties > >( [](){

		auto l = zuluMountTask::hiddenVolumeList() ;

		auto _validEntry = [ & ]( const QString& e ){

			if( e.startsWith( "/dev/md/md-device-map" ) ){

				return false ;
			}
			if( e.contains( "\tswap\t" ) || e.contains( "member\t" ) || e.contains( "\t/run/media/public" ) ){

				return false ;
			}
			if( !l.isEmpty() ){

				for( const auto& it : l ){

					if( e.startsWith( it + '\t' ) ){

						return false ;
					}
				}
			}
			return true ;
		} ;

		QVector< volumeEntryProperties > list ;

		auto all = utility::Task( utility::appendUserUID( "%1 -l" ).arg( zuluMountPath ),10000 ) ;

		if( all.finished() ){

			auto system = utility::Task( utility::appendUserUID( "%1 -S" ).arg( zuluMountPath ),10000 ) ;

			if( system.finished() ){

				auto a = all.splitOutput( '\n' ) ;
				auto s = system.splitOutput( '\n' ) ;

				for( const auto& it : a ){

					if( _validEntry( it ) ){

						const auto& e = utility::split( it,'\t' ) ;
						list.append( volumeEntryProperties( e,s.contains( e.first() ) ) ) ;
					}
				}
			}
		}

		return list ;
	} ) ;
}

void zuluMountTask::checkUnMount( const QString& volume )
{
	utility::Task( utility::appendUserUID( "%1 -c -d \"%2\"" ).arg( zuluMountPath,_device( volume ) ) ) ;
}

volumeStatus zuluMountTask::volumeMiniProperties( const QString& volume )
{
	auto _loopDeviceIsGone = []( const QString& device ){

		QFile f ;

		auto dev = QString( "%1\n" ).arg( device ) ;

		auto _match = [ & ]( const QString& path ){

			f.setFileName( QString( "/sys/block/%1/loop/backing_file" ).arg( path ) ) ;

			auto r = false ;

			if( f.open( QIODevice::ReadOnly ) ){

				r = f.readAll() == dev ;
			}

			f.close() ;

			return r ;
		} ;

		for( const auto& it : utility::directoryList( "/sys/block" ) ){

			if( it.startsWith( "loop" ) && _match( it ) ){

				return false ;
			}
		}

		return true ;
	} ;

	volumeStatus s{ volume,false,nullptr } ;

	if( !volume.startsWith( "UUID" ) && !volume.startsWith( "/dev/" ) ){

		/*
		 * There is some sort of a race condition here and things do not always work as expected
		 * try to sleep for a second to see if it will help
		 */

		utility::Task::waitForOneSecond() ;

		if( _loopDeviceIsGone( volume ) ){

			/*
			 * we were just asked to find properties of a loop device
			 * that no longer exists,remove it from the list in the GUI window
			 */

			s.volumeRemoved = true ;
			return s ;
		}
	}

	auto r = utility::Task( utility::appendUserUID( "%1 -L -d \"%2\"" ).arg( zuluMountPath,volume ) ) ;

	if( r.success() ){

		s.entry = new volumeEntryProperties( r.splitOutput( '\t' ),_volumeIsSystemVolume( volume ) ) ;
	}

	return s ;
}

volumeStatus zuluMountTask::deviceProperties( const zuluMountTask::event& deviceProperty )
{
	auto _mdRaidDevice = [ & ]( const QString& device ){

		auto d = _convert_md_raid_path( device,true ) ;

		volumeStatus s{ d,false,nullptr } ;

		if( deviceProperty.added ){

			s.entry = new volumeEntryProperties( _getVolumeProperties( d ) ) ;
		}else{
			s.volumeRemoved = true ;
		}

		return s ;
	} ;

	auto _dmDevice = [ & ]( const QString& device ){

		auto d = _convert_lvm_path( device ) ;

		volumeStatus s{ d,false,nullptr } ;

		if( deviceProperty.added ){

			s.entry = new volumeEntryProperties( _getVolumeProperties( d ) ) ;
		}else{
			s.volumeRemoved = true ;
		}

		return s ;
	} ;

	auto _normalDevice = [&]( const QString& device ){

		auto _allowed_device = []( const QString& device ){

			return	device.startsWith( "/dev/sd" )  ||
				device.startsWith( "/dev/hd" )  ||
				device.startsWith( "/dev/mmc" ) ;
		} ;

		volumeStatus s{ QString(),false,nullptr } ;

		if( _allowed_device( device ) ){

			s.volumeName = device ;

			if( deviceProperty.added ){

				s.entry = new volumeEntryProperties( _getVolumeProperties( device ) ) ;
			}else{
				s.volumeRemoved = true ;
			}
		}

		return s ;
	} ;

	auto _shouldNotGetHere = [](){

		return volumeStatus{ QString(),false,nullptr } ;
	} ;

	auto device = QString( "/dev/%1" ).arg( deviceProperty.volumeName ) ;

	switch( deviceProperty.deviceType ){

		case zuluMountTask::devices::device    : return _normalDevice( device ) ;
		case zuluMountTask::devices::md_device : return _mdRaidDevice( device ) ;
		case zuluMountTask::devices::dm_device : return _dmDevice( device )     ;
		default                                : return _shouldNotGetHere() ;
	}
}

static bool _delete_mount_point( const QString& m )
{
	auto mm = m ;
	mm.replace( "\"","\"\"\"" ) ;

	return utility::Task( utility::appendUserUID( "%1 -b \"%2\"" ).arg( zuluMountPath,mm ) ).success() ;
}

static bool _create_mount_point( const QString& m )
{
	auto mm = m ;
	mm.replace( "\"","\"\"\"" ) ;

	return utility::Task( utility::appendUserUID( "%1 -B \"%2\"" ).arg( zuluMountPath,mm ) ).success() ;
}

Task::future<bool>& zuluMountTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		auto _umount = [ & ](){

			auto mm = m ;
			mm.replace( "\"","\"\"\"" ) ;

			if( utility::Task( "fusermount -u \"" + mm + "\"",10000 ).success() ){

				return _delete_mount_point( m ) ;
			}else{
				return false ;
			}
		} ;

		for( int i = 0 ; i < 5 ; i++ ){

			if( _umount() ){

				return true ;
			}else{
				utility::Task::waitForOneSecond() ;
			}
		}

		return false ;
	} ) ;
}

using ev = zuluMountTask::encryptedVolume ;

Task::future< ev >& zuluMountTask::encryptedFolderMount( const QString& p,const QString& m,const QString& k,bool ro )
{
	return Task::run< ev >( [ p,m,k,ro ]()->ev{

		auto _cmd = [ & ]( const QString& e,ev::status status,const QString& arguments )->ev{

			for( const auto& it : { "/usr/local/bin/","/usr/local/sbin/","/usr/bin/","/usr/sbin/" } ){

				auto exe = it + e ;

				if( utility::pathExists( exe ) ){

					QProcess e ;

					e.start( exe + " " + arguments ) ;

					e.waitForStarted() ;

					e.write( k.toLatin1() + '\n' ) ;

					e.closeWriteChannel() ;

					if( e.waitForFinished( 10000 ) ){

						if( e.exitCode() == 0 ){

							return { ev::status::success } ;
						}else{
							return { status } ;
						}
					}else{
						return { ev::status::backendFail } ;
					}
				}
			}

			if( status == ev::status::cryfs ){

				return { ev::status::cryfsNotFound } ;
			}else{
				return { ev::status::encfsNotFound } ;
			}
		} ;

		auto _mount = [ & ]( std::function< ev() > unlocked )->ev{

			if( _create_mount_point( m ) ){

				auto e = unlocked() ;

				if( e.state != ev::status::success ) {

					_delete_mount_point( m ) ;
				}

				return e ;
			}else{
				return { ev::status::failedToCreateMountPoint } ;
			}
		} ;

		auto pp = p ;
		pp.replace( "\"","\"\"\"" ) ;

		auto mm = m ;
		mm.replace( "\"","\"\"\"" ) ;

		if( utility::pathExists( p + "/cryfs.config" ) ){

			return _mount( [ & ](){

				setenv( "CRYFS_NO_UPDATE_CHECK","TRUE",1 ) ;
				setenv( "CRYFS_FRONTEND","noninteractive",1 ) ;

				const char * opts ;

				if( ro ){

					opts = "\"%1\" \"%2\" -- -o ro -o fsname=cryfs@\"%3\" -o subtype=cryfs" ;
				}else{
					opts = "\"%1\" \"%2\" -- -o rw -o fsname=cryfs@\"%3\" -o subtype=cryfs" ;
				}

				return _cmd( "cryfs",ev::status::cryfs,QString( opts ).arg( pp,mm,pp ) ) ;
			} ) ;
		}

		if( utility::pathExists( p + "/.encfs6.xml" ) ){

			return _mount( [ & ](){

				const char * opts ;

				if( ro ){

					opts = "\"%1\" \"%2\" -S -o ro -o fsname=encfs@\"%3\" -o subtype=encfs" ;
				}else{
					opts = "\"%1\" \"%2\" -S -o rw -o fsname=encfs@\"%3\" -o subtype=encfs" ;
				}

				return _cmd( "encfs",ev::status::encfs,QString( opts ).arg( pp,mm,pp ) ) ;
			} ) ;
		}

		return { ev::status::unknown } ;
	} ) ;
}
