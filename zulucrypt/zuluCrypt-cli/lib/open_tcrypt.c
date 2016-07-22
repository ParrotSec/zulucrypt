/*
 *
 *  Copyright (c) 2012-2015
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

#include "includes.h"
#include <sys/syscall.h>
#include <libcryptsetup.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tcplay_support.h"
#include "check_tcrypt.h"

static void _chown( const char * x,uid_t y,gid_t z )
{
	if( chown( x,y,z ) ){;}
}
static void _chmod( const char * x,mode_t y )
{
	if( chmod( x,y ) ){;}
}
static void _write( int x,const void * y,size_t z )
{
	if( write( x,y,z ) ){;}
}
static void _close( int x )
{
	if( close( x ) ){;}
}

string_t zuluCryptCreateKeyFile( const char * key,size_t key_len,const char * fileName )
{
	string_t st = StringVoid ;
	int fd ;
	const char * file ;

	struct stat statstr ;

	if( key == NULL || key_len == 0 || fileName == NULL ){
		return StringVoid ;
	}

	#define path_does_not_exist( x ) stat( x,&statstr ) != 0

	if( path_does_not_exist( "/run" ) ){

		mkdir( "/run",S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH | S_IROTH ) ;
		_chown( "/run",0,0 ) ;
	}
	if( path_does_not_exist( "/run/zuluCrypt" ) ){

		mkdir( "/run/zuluCrypt",S_IRWXU ) ;
		_chown( "/run/zuluCrypt",0,0 ) ;
	}

	st = String_1( "/run/zuluCrypt/",fileName,NULL ) ;
	file = StringAppendInt( st,syscall( SYS_gettid ) ) ;
	fd = open( file,O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP |S_IROTH ) ;

	if( fd == -1 ){

		StringDelete( &st ) ;
	}else{
		_write( fd,key,key_len ) ;
		_close( fd ) ;
		_chown( file,0,0 ) ;
		_chmod( file,S_IRWXU ) ;
	}

	return st ;
}

string_t zuluCryptCreateKeyFile_1( string_t st,const char * fileName )
{
	return zuluCryptCreateKeyFile( StringContent( st ),StringLength( st ),fileName ) ;
}

static int _open_tcrypt_volume( const char * device,const resolve_path_t * opt )
{
	tc_api_task task ;
	int r = !TC_OK ;

	size_t i ;
	size_t k ;

	const char * const * z ;

	const char * e ;

	/*
	 * open_struct_t is defined in includes.h
	 */
	const open_struct_t * opts = opt->args ;

	string_t st = StringVoid ;

	if( tc_api_initialize() ){

		if( tc_api_task_initialize( &task,"map" ) ){

			tc_api_task_set( task,"veracrypt_mode",opts->veraCrypt_volume ) ;
			tc_api_task_set( task,"map_name",opts->mapper_name ) ;
			tc_api_task_set( task,"read_only",StringHasComponent( opts->m_opts,"ro" ) ) ;

			tc_api_task_set( task,"dev",device ) ;

			if( opts->tcrypt_system ){

				/*
				 * zuluCryptSystemVeraCryptPIM() is defined in create_tcrypt.c
				 */
				tc_api_task_set( task,"iteration_count",
						 zuluCryptSystemVeraCryptPIM( opts->iteration_count ) ) ;

				if( StringAtLeastOnePrefixMatch( device,"/dev/sd","/dev/hd",NULL ) ){

					st = String( device ) ;
					e = StringRemoveDigits( st ) ;
					tc_api_task_set( task,"sys",e ) ;
				}else{
					tc_api_task_set( task,"sys",device ) ;
				}
			}else{
				/*
				 * zuluCryptVeraCryptPIM() is defined in create_tcrypt.c
				 */
				tc_api_task_set( task,"iteration_count",
						 zuluCryptVeraCryptPIM( opts->iteration_count ) ) ;
			}

			tc_api_task_set( task,"passphrase",opts->key ) ;

			k = opts->tcrypt_keyfiles_count ;
			z = opts->tcrypt_keyfiles ;

			for( i = 0 ; i < k ; i++ ){

				tc_api_task_set( task,"keyfiles",*( z + i ) ) ;
			}

			r = tc_api_task_do( task ) ;

			tc_api_task_uninit( task ) ;

			StringDelete( &st ) ;
		}

		tc_api_uninit() ;
	}

	return r ;
}

static int _open_tcrypt_0( const open_struct_t * opt )
{
	/*
	 * zuluCryptResolveDevicePath_0() is defined in resolve_path.c
	 */
	return zuluCryptResolveDevicePath_0( _open_tcrypt_volume,opt,1 ) ;
}

int zuluCryptOpenTcrypt( const char * device,const char * mapper,const char * key,size_t key_len,
			 int key_source,int volume_type,const char * m_point,
			 uid_t uid,unsigned long m_flags,const char * fs_opts )
{
	open_struct_t opts ;
	string_t st ;
	int r ;
	const char * keyfile ;

	memset( &opts,'\0',sizeof( open_struct_t ) ) ;

	opts.device      = device ;
	opts.mapper_name = mapper ;
	opts.volume_type = volume_type ;
	opts.m_point     = m_point ;
	opts.uid         = uid ;
	opts.m_flags     = m_flags ;
	opts.fs_opts     = fs_opts ;

	if( m_flags & MS_RDONLY ){
		opts.m_opts = "ro" ;
	}else{
		opts.m_opts = "rw" ;
	}

	if( key_source == TCRYPT_KEYFILE ){
		st = zuluCryptCreateKeyFile( key,key_len,"open_tcrypt-" ) ;
		if( st != StringVoid ){

			keyfile = StringContent( st ) ;

			opts.tcrypt_keyfiles_count = 1 ;
			opts.tcrypt_keyfiles       = &keyfile ;

			r = zuluCryptOpenTcrypt_1( &opts ) ;
			/*
			 * zuluCryptDeleteFile() is defined in open_path_security.c
			 */
			zuluCryptDeleteFile( keyfile ) ;
			StringDelete( &st ) ;
		}else{
			r = 1 ;
		}
	}else if( key_source == TCRYPT_KEYFILE_FILE ){

		opts.tcrypt_keyfiles_count = 1 ;
		opts.tcrypt_keyfiles       = &key ;

		r = zuluCryptOpenTcrypt_1( &opts ) ;
	}else{
		opts.key_len = key_len ;
		opts.key     = key ;
		r = zuluCryptOpenTcrypt_1( &opts ) ;
	}

	return r ;
}

int zuluCryptOpenTcrypt_1( const open_struct_t * opts )
{
	/*
	 * zuluCryptOpenVolume_0() is defined in open_volume.c
	 */
	return zuluCryptOpenVolume_0( _open_tcrypt_0,opts ) ;
}

#if CHECK_TCRYPT

static int zuluExit( int st,struct crypt_device * cd )
{
	crypt_free( cd ) ;
	return st ;
}
/*
 * 1 is returned if a volume is a truecrypt volume.
 * 0 is returned if a volume is not a truecrypt volume or functionality is not supported
 */
int zuluCryptVolumeIsTcrypt( const char * device,const char * key,int key_source )
{
	struct crypt_device * cd = NULL;
	struct crypt_params_tcrypt params ;

	memset( &params,'\0',sizeof( struct crypt_params_tcrypt ) ) ;

	if( key_source ){;}

	if( crypt_init( &cd,device ) < 0 ){
		return 0 ;
	}else{
		params.passphrase      = key ;
		params.passphrase_size = StringSize( key ) ;
		params.flags           = CRYPT_TCRYPT_LEGACY_MODES ;

		if( crypt_load( cd,CRYPT_TCRYPT,&params ) == 0 ){
			return zuluExit( 1,cd ) ;
		}else{
			return zuluExit( 0,cd ) ;
		}
	}
}

#else

int zuluCryptVolumeIsTcrypt( const char * device,const char * key,int key_source )
{
	if( 0 && device && key && key_source ){;}

	return 1 ;
}

#endif
