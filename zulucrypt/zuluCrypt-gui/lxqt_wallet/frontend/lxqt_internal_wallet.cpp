/*
 * copyright: 2013-2015
 * name : Francis Banyikwa
 * email: mhogomchungu@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "lxqt_internal_wallet.h"

namespace Task = LxQt::Wallet::Task ;

LxQt::Wallet::internalWallet::internalWallet() : m_wallet( 0 )
{
}

LxQt::Wallet::internalWallet::~internalWallet()
{
	lxqt_wallet_close( &m_wallet ) ;
}

void LxQt::Wallet::internalWallet::setImage( const QString& image )
{
	m_image = image ;
	this->setWindowIcon( QIcon( image ) ) ;
}

void LxQt::Wallet::internalWallet::openWallet( QString password )
{
	m_password = password ;

	Task::run< lxqt_wallet_error >( [ this ](){

		return lxqt_wallet_open( &m_wallet,
					 m_password.toLatin1().constData(),
					 m_password.size(),m_walletName.toLatin1().constData(),
					 m_applicationName.toLatin1().constData() ) ;

	} ).then( [ this ]( lxqt_wallet_error r ){

		this->opened( r == lxqt_wallet_no_error ) ;
	} ) ;
}

void LxQt::Wallet::internalWallet::opened( bool opened )
{
	m_correctPassword( opened ) ;

	m_opened = opened ;

	if( m_opened ){

		m_loop.exit() ;

		emit getPassWord( m_password ) ;
		emit walletIsOpen( m_opened ) ;
	}
}

bool LxQt::Wallet::internalWallet::await_open( const QString& walletName,const QString& applicationName,
					       const QString& password,const QString& displayApplicationName )
{
	this->open( walletName,applicationName,password,displayApplicationName ) ;

	m_loop.exec() ;

	return m_opened ;
}

void LxQt::Wallet::internalWallet::open( const QString& walletName,const QString& applicationName,
					 const QString& password,const QString& displayApplicationName )
{
	m_walletName      = walletName ;
	m_applicationName = applicationName ;
	m_password        = password ;

	if( m_applicationName.isEmpty() ){

		m_applicationName = m_walletName ;
	}

	if( displayApplicationName.isEmpty() ){

		m_displayApplicationName = m_applicationName ;
	}else{
		m_displayApplicationName = displayApplicationName ;
	}

	if( LxQt::Wallet::walletExists( LxQt::Wallet::internalBackEnd,m_walletName,m_applicationName ) ){

		this->openWallet() ;
	}else{
		this->createWallet() ;
	}
}

void LxQt::Wallet::internalWallet::openWallet()
{
	if( m_password.isEmpty() ){

		/*
		 * to prevent an unnecessary prompt,try to open a wallet without a password and then
		 * prompt on failure,this will allow a silent opening of the wallet set without a password.
		 */

		Task::run< lxqt_wallet_error >( [ this ](){

			return lxqt_wallet_open( &m_wallet,
						 m_password.toLatin1().constData(),
						 m_password.size(),
						 m_walletName.toLatin1().constData(),
						 m_applicationName.toLatin1().constData() ) ;

		} ).then( [ this ]( lxqt_wallet_error r ){

			if( r == lxqt_wallet_no_error ){

				this->opened( true ) ;
			}else{
				/*
				 * passwordless opening failed,prompt a user for a password
				 */

				using pwd = LxQt::Wallet::password_dialog ;

				auto _cancelled = [ this ](){

					m_opened = false ;

					m_loop.exit() ;

					emit walletIsOpen( false ) ;
				} ;

				pwd::instance( this,
					       m_walletName,
					       m_displayApplicationName,
					       [ this ]( const QString& p ){ this->openWallet( p ) ; },
					       std::move( _cancelled ),
					       &m_correctPassword ) ;
			}
		} ) ;
	}else{
		this->openWallet( m_password ) ;
	}
}

void LxQt::Wallet::internalWallet::createWallet()
{
	using cbd = LxQt::Wallet::changePassWordDialog ;

	const auto& w = m_walletName ;
	const auto& d = m_displayApplicationName ;

	cbd::instance( this,w,d,[ this ]( const QString& password,bool create ){

		if( create ){

			m_password = password ;

			Task::run< lxqt_wallet_error >( [ this ](){

				return lxqt_wallet_create( m_password.toLatin1().constData(),
							   m_password.size(),
							   m_walletName.toLatin1().constData(),
							   m_applicationName.toLatin1().constData() ) ;

			} ).then( [ this ]( lxqt_wallet_error r ){

				if( r == lxqt_wallet_no_error ){

					this->openWallet( m_password ) ;
				}else{
					emit walletIsOpen( false ) ;
				}
			} ) ;
		}else{
			emit walletIsOpen( false ) ;
		}
	} ) ;
}

void LxQt::Wallet::internalWallet::changeWalletPassWord( const QString& walletName,const QString& applicationName )
{
	LxQt::Wallet::changePassWordDialog::instance_1( this,walletName,applicationName,[ this ]( bool c ){

		QMetaObject::invokeMethod( m_interfaceObject,"walletpassWordChanged",Q_ARG( bool,c ) ) ;
	} ) ;
}

QByteArray LxQt::Wallet::internalWallet::readValue( const QString& key )
{
	lxqt_wallet_key_values_t key_value ;

	if( lxqt_wallet_read_key_value( m_wallet,key.toLatin1().constData(),key.size() + 1,&key_value ) ){

		return QByteArray( key_value.key_value,key_value.key_value_size ) ;
	}else{
		return QByteArray() ;
	}
}

QVector<LxQt::Wallet::walletKeyValues> LxQt::Wallet::internalWallet::readAllKeyValues( void )
{
	QVector<LxQt::Wallet::walletKeyValues> w ;

	lxqt_wallet_iterator_t iter ;

	iter.iter_pos = 0 ;

	#define e QByteArray( iter.entry.key,iter.entry.key_size - 1 )
	#define f QByteArray( iter.entry.key_value,iter.entry.key_value_size )

	while( lxqt_wallet_iter_read_value( m_wallet,&iter ) ){

		w.append( LxQt::Wallet::walletKeyValues( e,f ) ) ;
	}

	return w ;
}

QStringList LxQt::Wallet::internalWallet::readAllKeys()
{
	QStringList l ;
	lxqt_wallet_iterator_t iter ;

	iter.iter_pos = 0 ;

	while( lxqt_wallet_iter_read_value( m_wallet,&iter ) ){

		l.append( QByteArray( iter.entry.key,iter.entry.key_size - 1 ) ) ;
	}

	return l ;
}

bool LxQt::Wallet::internalWallet::addKey( const QString& key,const QByteArray& value )
{
	/*
	 * For the key,we add +1 to the key size to include the '\0' character in the key to
	 * avoid possible collisions if our keys prefix match
	 */
	auto r = lxqt_wallet_add_key( m_wallet,key.toLatin1().constData(),key.size() + 1,value.constData(),value.size() ) ;
	return r == lxqt_wallet_no_error ;
}

void LxQt::Wallet::internalWallet::deleteKey( const QString& key )
{
	lxqt_wallet_delete_key( m_wallet,key.toLatin1().constData(),key.size() + 1 ) ;
}

int LxQt::Wallet::internalWallet::walletSize( void )
{
	return lxqt_wallet_wallet_size( m_wallet ) ;
}

void LxQt::Wallet::internalWallet::closeWallet( bool b )
{
	Q_UNUSED( b ) ;
	lxqt_wallet_close( &m_wallet ) ;
}

LxQt::Wallet::walletBackEnd LxQt::Wallet::internalWallet::backEnd()
{
	return LxQt::Wallet::internalBackEnd ;
}

bool LxQt::Wallet::internalWallet::walletIsOpened()
{
	return m_wallet != 0 ;
}

void LxQt::Wallet::internalWallet::setInterfaceObject( QWidget * interfaceObject )
{
	this->setParent( interfaceObject ) ;
	m_interfaceObject = interfaceObject ;
	connect( this,SIGNAL( walletIsOpen( bool ) ),m_interfaceObject,SLOT( walletIsOpen( bool ) ) ) ;
}

QObject * LxQt::Wallet::internalWallet::qObject()
{
	this->setObjectName( m_password ) ;
	return this ;
}

QString LxQt::Wallet::internalWallet::storagePath()
{
	return QString() ;
}

QStringList LxQt::Wallet::internalWallet::managedWalletList()
{
	char path[ 4096 ] ;
	lxqt_wallet_application_wallet_path( path,4096,m_applicationName.toLatin1().constData() ) ;
	QDir d( path ) ;
	QStringList l = d.entryList() ;
	l.removeOne( "." ) ;
	l.removeOne( ".." ) ;

	if( l.size() > 0 ){
		/*
		 * remove the extension part of a file name
		 */
		const QString& q = l.at( 0 ) ;
		l.replaceInStrings( q.mid( q.indexOf( "." ) ),"" ) ;
	}

	return l ;
}

QString LxQt::Wallet::internalWallet::localDefaultWalletName()
{
	return QString() ;
}

QString LxQt::Wallet::internalWallet::networkDefaultWalletName()
{
	return QString() ;
}
