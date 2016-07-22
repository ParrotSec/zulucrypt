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
#include <sys/ioctl.h>
#include <linux/loop.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int zuluExit( int result,string_t st,int fd_loop,int fd_path )
{
	if( st == 0 ){

		StringDelete( &st ) ;

		if( fd_loop != -1 ){

			close( fd_loop ) ;
		}
	}
	if( fd_path != -1 ){

		close( fd_path ) ;
	}
	return result ;
}

static string_t _StringGetFromVirtualFile( string_t * st )
{
	string_t xt = StringGetFromVirtualFile( StringContent( *st ) ) ;
	StringDelete( st ) ;
	return xt ;
}

string_t zuluCryptLoopDeviceAddress_2( const char * device )
{
	int fd ;
	char * path ;
	struct loop_info64 l_info ;

	string_t st = String_1( "/sys/block/",device + 5,"/loop/backing_file",NULL ) ;

	string_t xt = _StringGetFromVirtualFile( &st ) ;

	if( xt == StringVoid ){

		memset( &l_info,'\0',sizeof( struct loop_info64 ) ) ;

		fd = open( device,O_RDONLY ) ;

		ioctl( fd,LOOP_GET_STATUS64,&l_info ) ;

		path = zuluCryptRealPath( ( char * ) l_info.lo_file_name ) ;

		close( fd ) ;

		xt = StringInherit( &path ) ;
	}else{
		StringRemoveRight( xt,1 ) ;
	}
	/*
	 * zuluCryptEncodeMountEntry() is defined in mount_volume.c
	 */
	zuluCryptEncodeMountEntry( xt ) ;
	return xt ;
}

char * zuluCryptLoopDeviceAddress( const char * device )
{
	string_t st = zuluCryptLoopDeviceAddress_2( device ) ;
	return StringDeleteHandle( &st ) ;
}

char * zuluCryptLoopDeviceAddress_1( const char * device )
{
	int fd ;
	char * path ;
	struct loop_info64 l_info ;

	string_t st = String_1( "/sys/block/",device + 5,"/loop/backing_file",NULL ) ;

	string_t xt = _StringGetFromVirtualFile( &st ) ;

	if( xt == StringVoid ){

		memset( &l_info,'\0',sizeof( struct loop_info64 ) ) ;

		fd = open( device,O_RDONLY ) ;

		ioctl( fd,LOOP_GET_STATUS64,&l_info ) ;

		path = zuluCryptRealPath( ( char * ) l_info.lo_file_name ) ;

		close( fd ) ;

		return path ;
	}else{
		StringRemoveRight( xt,1 ) ;
		return StringDeleteHandle( &xt ) ;
	}
}

char * zuluCryptGetALoopDeviceAssociatedWithAnImageFile( const char * path )
{
	int i ;
	string_t st = String( "" ) ;
	const char * e ;
	char * f ;

	for( i = 0 ; i < 255 ; i++ ){

		StringReplace( st,"/dev/loop" ) ;

		e = StringAppendInt( st,i ) ;

		f = zuluCryptLoopDeviceAddress_1( e ) ;

		if( StringsAreEqual( path,f ) ){

			StringFree( f ) ;

			return StringDeleteHandle( &st ) ;
		}else{
			StringFree( f ) ;
		}
	}

	StringDelete( &st ) ;
	return NULL ;
}

char * zuluCryptGetLoopDeviceAddress( const char * device )
{
	char * z = NULL ;
	const char * e ;

	string_t st = StringVoid ;
	string_t xt = StringVoid ;

	int i ;
	int r ;

	z = zuluCryptLoopDeviceAddress_1( device ) ;

	if( z == NULL ){
		return NULL ;
	}else{
		st = String( "" ) ;

		for( i = 0 ; i < 255 ; i++ ){

			StringReplace( st,"/sys/block/loop" ) ;
			StringAppendInt( st,i ) ;

			xt = StringGetFromVirtualFile( StringAppend( st,"/loop/backing_file" ) ) ;

			e = StringRemoveRight( xt,1 ) ;
			r = StringsAreEqual( e,z ) ;

			StringDelete( &xt ) ;

			if( r ){

				StringReplace( st,"/dev/loop" ) ;
				e = StringAppendInt( st,i ) ;

				if( StringsAreNotEqual( device,e ) ){

					break ;
				}
			}else{
				StringReset( st ) ;
			}
		}

		StringFree( z ) ;

		if( StringIsEmpty( st ) ){

			StringDelete( &st ) ;
			return NULL ;
		}else{
			return StringDeleteHandle( &st ) ;
		}
	}
}

char * zuluCryptGetFileNameFromFileDescriptor( int fd )
{
	char * e ;
	char * c ;

	string_t xt = String( "/proc/self/fd/" ) ;

	e = zuluCryptRealPath( StringAppendInt( xt,fd ) ) ;

	/*
	 * zuluCryptResolvePath_4() is defined in resolve_paths.c
	 */
	c = zuluCryptResolvePath_4( e ) ;

	StringFree( e ) ;

	StringDelete( &xt ) ;

	return c ;
}

/*
 * Here,we check if the path we sent to open() is the path open() used. This check is necessary to
 * guard against some known hypothetical exploits
 */
static int _paths_are_not_sane( int fd,const char * path )
{
	char * c = zuluCryptGetFileNameFromFileDescriptor( fd ) ;
	int st = StringsAreNotEqual( c,path ) ;
	StringFree( c ) ;
	return st ;
}

static int open_loop_device_1( string_t * loop_device )
{
	string_t st = String( "" ) ;
	int i ;
	int fd ;
	const char * path ;
	struct loop_info64 l_info ;
	int r = 0 ;

	for( i = 0 ; i < 255 ; i++ ){

		StringReplace( st,"/dev/loop" ) ;
		path = StringAppendInt( st,i ) ;
		fd = open( path,O_RDONLY ) ;

		if( fd == -1 ){
			r = 0 ;
			break ;
		}
		if( ioctl( fd,LOOP_GET_STATUS64,&l_info ) != 0 ){

			if( errno == ENXIO) {
				*loop_device = st ;
				close( fd ) ;
				r = 1 ;
				break ;
			}
		}
		close( fd ) ;
	}
	StringDelete( &st ) ;
	return r ;
}

static int open_loop_device( string_t * loop_device )
{
	int devnr ;
	int fd_loop ;

	fd_loop = open( "/dev/loop-control",O_RDONLY ) ;

	if( fd_loop == -1 ){

		return open_loop_device_1( loop_device ) ;
	}else{
		devnr = ioctl( fd_loop,LOOP_CTL_GET_FREE ) ;
		close( fd_loop ) ;

		if( devnr < 0 ){

			return open_loop_device_1( loop_device ) ;
		}else{
			*loop_device = String( "/dev/loop" ) ;
			StringAppendInt( *loop_device,devnr ) ;
			return 1 ;
		}
	}
}

static int attach_device_to_loop( int fd_path,int * fd_loop,
				  string_t loop_device,int mode )
{
	char * path ;
	size_t size ;

	struct loop_info64 l_info ;

	*fd_loop = open( StringContent( loop_device ),mode ) ;

	memset( &l_info,'\0',sizeof( struct loop_info64 ) ) ;

	if( *fd_loop == -1 ){
		return 0 ;
	}
	if( ioctl( *fd_loop,LOOP_SET_FD,fd_path ) == -1 ){
		return 0 ;
	}
	if( ioctl( *fd_loop,LOOP_GET_STATUS64,&l_info ) == -1 ){
		return 0;
	}

	l_info.lo_flags |= LO_FLAGS_AUTOCLEAR;

	path = zuluCryptGetFileNameFromFileDescriptor( fd_path ) ;

	if( path == NULL ){
		return 0 ;
	}else{
		size = sizeof( l_info.lo_file_name ) ;
		strncpy( ( char * )l_info.lo_file_name,path,size ) ;
		l_info.lo_file_name[ size - 1 ] = '\0' ;
		free( path ) ;

		if( ioctl( *fd_loop,LOOP_SET_STATUS64,&l_info ) == -1 ){
			return 0 ;
		}else{
			return 1 ;
		}
	}
}

static int _attach_loop_device_to_file( const char * path,int mode,
					int * loop_fd,string_t * loop_device )
{
	string_t loopd = StringVoid ;

	int fd_loop = -1 ;
	int fd_path = -1 ;

	if( !open_loop_device( &loopd ) ){
		return zuluExit( 0,loopd,fd_loop,fd_path ) ;
	}

	fd_path = open( path,mode ) ;

	if( fd_path == -1 ){
		return zuluExit( 0,loopd,fd_loop,fd_path ) ;
	}

	fcntl( fd_path,F_SETFD,FD_CLOEXEC ) ;

	if( _paths_are_not_sane( fd_path,path ) ){
		return zuluExit( 0,loopd,fd_loop,fd_path ) ;
	}
	if( attach_device_to_loop( fd_path,&fd_loop,loopd,mode ) ){
		*loop_device = loopd ;
		*loop_fd = fd_loop ;
		return zuluExit( 1,loopd,fd_loop,fd_path ) ;
	}else{
		return zuluExit( 0,loopd,fd_loop,fd_path ) ;
	}
}

int zuluCryptAttachLoopDeviceToFile( const char * path,int mode,
				     int * loop_fd,string_t * loop_device )
{
	int i ;
	int j ;
	/*
	 * try to attach a loop device multiple times
	 */
	for( j = 0 ; j < 3 ; j++ ){
		i = _attach_loop_device_to_file( path,mode,loop_fd,loop_device ) ;
		if( i == 1 ){
			break ;
		}else{
			sleep( 1 ) ;
		}
	}
	return i ;
}

static int _attach_loop_device( int fd_path,int * fd_loop,int mode,string_t * loop_device )
{
	string_t loopd = StringVoid ;

	if( !open_loop_device( &loopd ) ){
		return 0 ;
	}else{
		if( attach_device_to_loop( fd_path,fd_loop,loopd,mode ) ){
			*loop_device = loopd ;
			return 1 ;
		}else{
			return 0 ;
		}
	}
}

int zuluCryptAttachLoopDeviceToFileUsingFileDescriptor( int fd_path,
							int * fd_loop,
							int mode,
							string_t * loop_device )
{
	int i ;
	int j ;
	/*
	 * try to attach a loop device multiple times
	 */
	for( j = 0 ; j < 3 ; j++ ){

		i = _attach_loop_device( fd_path,fd_loop,mode,loop_device ) ;

		if( i == 1 ){
			break ;
		}else{
			sleep( 1 ) ;
		}
	}
	return i ;
}
