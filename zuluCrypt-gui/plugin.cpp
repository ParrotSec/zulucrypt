/*
 *
 *  Copyright ( c ) 2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "plugin.h"
#include "task.h"

#include "../plugins/plugins.h"

#include "ui_plugin.h"

#include "utility.h"
#include "dialogmsg.h"

#include <QCloseEvent>
#include <QEvent>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QIcon>

#include <memory>

plugin::plugin( QWidget * parent,plugins::plugin t,std::function< void( const QByteArray& ) > function,const QString& e,const QVector<QString>& exe ) :
	QDialog( parent ),m_ui( new Ui::plugin ),m_function( std::move( function ) ),m_pluginType( t ),m_exe( exe )
{
	m_ui->setupUi( this ) ;

	if( !e.isEmpty() ){

		m_ui->label->setText( e ) ;
	}

	this->setFixedSize( this->size() ) ;
	this->setFont( parent->font() ) ;

	connect( m_ui->pbSetKey,SIGNAL( clicked() ),this,SLOT( pbSetKey() ) ) ;
	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( pbClose() ) ) ;
	connect( m_ui->pbKeyFile,SIGNAL( clicked() ),this,SLOT( pbSelectKeyFile() ) ) ;

	m_ui->pbKeyFile->setIcon( QIcon( ":/file.png" ) ) ;

	m_ui->lineEdit->setFocus() ;

	this->ShowUI() ;
}

bool plugin::eventFilter( QObject * watched,QEvent * event )
{
	return utility::eventFilter( this,watched,event,[ this ](){ this->HideUI() ; } ) ;
}

void plugin::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->HideUI() ;
}

plugin::~plugin()
{
	delete m_ui ;
}

void plugin::ShowUI()
{
	this->show() ;
}

void plugin::HideUI()
{
	m_function( m_key ) ;

	this->hide() ;
	this->deleteLater() ;
}

void plugin::pbSetKey()
{
	auto passphrase = m_ui->lineEdit->text() ;
	auto keyFile    = m_ui->lineEdit_2->text() ;

	DialogMsg msg( this ) ;

	if( keyFile.isEmpty() ){

		return msg.ShowUIOK( tr( "ERROR" ),tr( "KeyFile Not Set" ) ) ;
	}

	this->disableAll() ;

	Task::run< QByteArray >( [ this,passphrase,keyFile ](){

		switch( m_pluginType ){
		case plugins::plugin::gpg:

			return plugins::gpg( m_exe,keyFile,passphrase ) ;

		case plugins::plugin::hmac_key_1:

			return plugins::hmac_key_1( m_exe,keyFile,passphrase ) ;

		case plugins::plugin::hmac_key:

			return plugins::hmac_key( keyFile,passphrase ) ;

		case plugins::plugin::keyKeyFile:

			return plugins::keyKeyFile( m_exe,keyFile,passphrase ) ;

		case plugins::plugin::luks:

			return plugins::luks( m_exe,keyFile,passphrase ) ;

		case plugins::plugin::steghide:

			return plugins::steghide( m_exe,keyFile,passphrase ) ;
		default:
			return QByteArray() ;
		}

	} ).then( [ this ]( const QByteArray& e ){

		m_key = e ;

		if( m_key.isEmpty() ){

			DialogMsg msg( this ) ;

			msg.ShowUIOK( tr( "ERROR" ),tr( "Failed To Generate Key" ) ) ;

			this->enableAll() ;
		}else{
			this->HideUI() ;
		}
	} ) ;
}

void plugin::pbSelectKeyFile()
{
	m_ui->lineEdit_2->setText( QFileDialog::getOpenFileName( this,tr( "KeyFile" ),utility::homePath() ) ) ;
}

void plugin::pbClose()
{
	this->HideUI() ;
}

void plugin::enableAll()
{
	m_ui->pbCancel->setEnabled( true ) ;
	m_ui->pbKeyFile->setEnabled( true ) ;
	m_ui->pbSetKey->setEnabled( true ) ;
	m_ui->groupBox->setEnabled( true ) ;
	m_ui->label->setEnabled( true ) ;
	m_ui->label_2->setEnabled( true ) ;
	m_ui->label_3->setEnabled( true ) ;
	m_ui->lineEdit->setEnabled( true ) ;
	m_ui->lineEdit_2->setEnabled( true ) ;
}

void plugin::disableAll()
{
	m_ui->pbCancel->setEnabled( false ) ;
	m_ui->pbKeyFile->setEnabled( false ) ;
	m_ui->pbSetKey->setEnabled( false ) ;
	m_ui->groupBox->setEnabled( false ) ;
	m_ui->label->setEnabled( false ) ;
	m_ui->label_2->setEnabled( false ) ;
	m_ui->label_3->setEnabled( false ) ;
	m_ui->lineEdit->setEnabled( false ) ;
	m_ui->lineEdit_2->setEnabled( false ) ;
}
