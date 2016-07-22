/*
 *  Copyright ( c ) 2012-2015
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

#include "walletconfiginput.h"
#include "ui_walletconfiginput.h"

#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>

#include "dialogmsg.h"
#include "utility.h"
#include "openvolume.h"
#include <QDebug>

walletconfiginput::walletconfiginput( QWidget * parent,
				      std::function< void( const QString&,const QString&,const QString& ) > p,
				      std::function< void() > q ) :
	QDialog( parent ),m_ui( new Ui::walletconfiginput ),m_add( std::move( p ) ),m_cancel( std::move( q ) )
{
	m_ui->setupUi( this ) ;

	this->setFixedSize( this->size() ) ;

	if( parent ){

		this->setFont( parent->font() ) ;
	}

	m_ui->lineEditVolumeID->setEnabled( true ) ;
	m_ui->lineEditKey->setEchoMode( QLineEdit::Password ) ;
	m_ui->lineEditRepeatKey->setEchoMode( QLineEdit::Password ) ;

	connect( m_ui->pushButtonAdd,SIGNAL( clicked() ),this,SLOT( pbAdd() ) ) ;
	connect( m_ui->pushButtonCancel,SIGNAL( clicked() ),this,SLOT( slotCancel() ) ) ;
	connect( m_ui->pushButtonImageFile,SIGNAL( clicked() ),this,SLOT( pbImageFilePath() ) ) ;
	connect( m_ui->pushButtonVolume,SIGNAL( clicked() ),this,SLOT( pbVolumePath() ) ) ;

	m_ui->pushButtonImageFile->setIcon( QIcon( ":/file.png" ) ) ;
	m_ui->pushButtonVolume->setIcon( QIcon( ":/partition.png" ) ) ;

	this->installEventFilter( this ) ;

	this->show() ;
}

bool walletconfiginput::eventFilter( QObject * watched,QEvent * event )
{
	return utility::eventFilter( this,watched,event,[ this ](){ this->HideUI() ; } ) ;
}

walletconfiginput::~walletconfiginput()
{
	delete m_ui ;
}

void walletconfiginput::ShowUI()
{
	this->show() ;
}

void walletconfiginput::HideUI()
{
	this->hide() ;
	this->deleteLater() ;
}

void walletconfiginput::pbAdd()
{
	auto volumeID  = m_ui->lineEditVolumeID->text() ;
	auto comment   = m_ui->lineEditComment->text()  ;
	auto key       = m_ui->lineEditKey->text()      ;
	auto repeatKey = m_ui->lineEditRepeatKey->text() ;

	if( volumeID.isEmpty() || key.isEmpty() || repeatKey.isEmpty() ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR!" ),tr( "Atleast one required field is empty" ) ) ;

		return ;
	}

	if( key != repeatKey ){

		DialogMsg msg( this ) ;

		msg.ShowUIOK( tr( "ERROR!" ),tr( "Passphrases do not match" ) ) ;

		return ;
	}

	if( comment.isEmpty() ){

		comment = "Nil" ;
	}

	m_add( volumeID,comment,key ) ;

	this->HideUI() ;
}

void walletconfiginput::slotCancel()
{
	m_cancel() ;
	this->HideUI() ;
}

void walletconfiginput::pbImageFilePath()
{
	auto x = QFileDialog::getOpenFileName( this,tr( "Select A Volume" ),utility::homePath(),0 ) ;

	if( !x.isEmpty() ){

		this->setvolumeID( x ) ;
	}
}

void walletconfiginput::pbVolumePath()
{
	openvolume::instance( this ).showEncryptedOnly().ShowAllPartitions( [ this ]( const QString& e ){

		this->setvolumeID( e ) ;
	} ) ;
}

void walletconfiginput::setvolumeID( QString id )
{
	if( id.startsWith( "UUID=" ) ){

		m_ui->lineEditVolumeID->setText( id ) ;
	}else{
		auto z = Task::await<QString>( utility::getUUIDFromPath( id ) ) ;

		if( z.isEmpty() ){

			m_ui->lineEditVolumeID->setText( utility::getVolumeID( id ) ) ;
		}else{
			m_ui->lineEditVolumeID->setText( z ) ;
		}
	}

	this->focus() ;
}

void walletconfiginput::focus()
{
	if( m_ui->lineEditComment->text().isEmpty() ){

		m_ui->lineEditComment->setFocus() ;

	}else if( m_ui->lineEditKey->text().isEmpty() ){

		m_ui->lineEditKey->setFocus() ;

	}else if( m_ui->lineEditRepeatKey->text().isEmpty() ){

		m_ui->lineEditRepeatKey->setFocus() ;
	}else{
		m_ui->pushButtonAdd->setFocus() ;
	}
}

void walletconfiginput::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	m_cancel() ;
	this->HideUI() ;
}
