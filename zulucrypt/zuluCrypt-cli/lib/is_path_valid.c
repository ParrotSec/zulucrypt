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
#include <sys/stat.h>

int zuluCryptPathIsValid( const char * path )
{
	struct stat st ;
	return stat( path,&st ) == 0 ;
}

int zuluCryptPathIsNotValid( const char * path )
{
	struct stat st ;
	return stat( path,&st ) != 0 ;
}

int zuluCryptPathIsNotValid_0( string_t path )
{
	return zuluCryptPathIsNotValid( StringContent( path ) ) ;
}
