/*
 *
 *  Copyright ( c ) 2011-2015
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

#include "favorites.h"
#include "ui_favorites.h"
#include <iostream>

#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

#include "openvolume.h"
#include "utility.h"
#include "dialogmsg.h"
#include "tablewidget.h"

favorites::favorites( QWidget * parent,bool e ) : QDialog( parent ),m_ui( new Ui::favorites )
{
	m_ui->setupUi( this ) ;

	this->setWindowFlags( Qt::Window | Qt::Dialog ) ;
	this->setFont( parent->font() ) ;

	this->setFixedSize( this->size() ) ;

	connect( m_ui->pbFolderAddress,SIGNAL( clicked() ),this,SLOT( folderAddress() ) ) ;
	connect( m_ui->pbDeviceAddress,SIGNAL( clicked() ),this,SLOT( deviceAddress() ) ) ;
	connect( m_ui->pbAdd,SIGNAL( clicked() ),this,SLOT( add() ) ) ;
	connect( m_ui->pbFileAddress,SIGNAL( clicked() ),this,SLOT( fileAddress() ) ) ;
	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( cancel() ) ) ;
	connect( m_ui->tableWidget,SIGNAL( currentItemChanged( QTableWidgetItem *,QTableWidgetItem * ) ),this,
		SLOT( currentItemChanged( QTableWidgetItem *,QTableWidgetItem * ) ) ) ;
	connect( m_ui->tableWidget,SIGNAL( itemClicked( QTableWidgetItem * ) ),this,
		SLOT( itemClicked( QTableWidgetItem * ) ) ) ;
	connect( m_ui->lineEditDeviceAddress,SIGNAL( textChanged( QString ) ),this,SLOT( devicePathTextChange( QString ) ) ) ;

	m_ui->pbFileAddress->setIcon( QIcon( ":/file.png" ) ) ;
	m_ui->pbDeviceAddress->setIcon( QIcon( ":/partition.png" ) ) ;
	m_ui->pbFolderAddress->setIcon( QIcon( ":/folder.png" ) ) ;

	m_ui->pbFolderAddress->setEnabled( e ) ;

	this->addAction( [ this ](){

		auto ac = new QAction( this ) ;

		QList<QKeySequence> keys ;

		keys.append( Qt::Key_Enter ) ;
		keys.append( Qt::Key_Return ) ;
		keys.append( Qt::Key_Menu ) ;

		ac->setShortcuts( keys ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( shortcutPressed() ) ) ;

		return ac ;
	}() ) ;

	this->installEventFilter( this ) ;

	this->ShowUI() ;
}

bool favorites::eventFilter( QObject * watched,QEvent * event )
{
	return utility::eventFilter( this,watched,event,[ this ](){ this->HideUI() ; } ) ;
}

void favorites::devicePathTextChange( QString txt )
{
	if( txt.isEmpty() ){

		m_ui->lineEditMountPath->clear() ; ;
	}else{
		auto s = txt.split( "/" ).last() ;

		if( s.isEmpty() ){

			m_ui->lineEditMountPath->setText( txt ) ;
		}else{
			m_ui->lineEditMountPath->setText( s ) ;
		}
	}
}

void favorites::shortcutPressed()
{
	this->itemClicked( m_ui->tableWidget->currentItem(),false ) ;
}

void favorites::deviceAddress()
{
	openvolume::instance( this ).ShowAllPartitions( [ this ]( const QString& e ){

		m_ui->lineEditDeviceAddress->setText( e ) ;
	} ) ;
}

void favorites::ShowUI()
{
	m_ui->tableWidget->setColumnWidth( 0,285 ) ;
	m_ui->tableWidget->setColumnWidth( 1,285 ) ;

	tablewidget::clearTable( m_ui->tableWidget ) ;

	auto _add_entry = [ this ]( const QStringList& l ){

		if( l.size() > 1 ){

			this->addEntries( l ) ;
		}
	} ;

	for( const auto& it : utility::readFavorites() ){

		_add_entry( utility::split( it,'\t' ) ) ;
	}

	m_ui->lineEditDeviceAddress->clear() ;
	m_ui->lineEditMountPath->clear() ;
	m_ui->tableWidget->setFocus() ;

	this->show() ;
}

void favorites::HideUI()
{
	this->hide() ;
	this->deleteLater() ;
}

void favorites::addEntries( const QStringList& l )
{
	tablewidget::addRow( m_ui->tableWidget,l ) ;
}

void favorites::itemClicked( QTableWidgetItem * current )
{
	this->itemClicked( current,true ) ;
}

void favorites::itemClicked( QTableWidgetItem * current,bool clicked )
{
	QMenu m ;
	m.setFont( this->font() ) ;
	connect( m.addAction( tr( "Remove Selected Entry" ) ),SIGNAL( triggered() ),this,SLOT( removeEntryFromFavoriteList() ) ) ;

	m.addSeparator() ;
	m.addAction( tr( "Cancel" ) ) ;

	if( clicked ){

		m.exec( QCursor::pos() ) ;
	}else{
		int x = m_ui->tableWidget->columnWidth( 0 ) ;
		int y = m_ui->tableWidget->rowHeight( current->row() ) * current->row() + 20 ;
		m.exec( m_ui->tableWidget->mapToGlobal( QPoint( x,y ) ) ) ;
	}
}

void favorites::removeEntryFromFavoriteList()
{
	auto table = m_ui->tableWidget ;

	table->setEnabled( false ) ;

	auto row = table->currentRow() ;

	auto p = table->item( row,0 )->text() ;
	auto q = table->item( row,1 )->text() ;

	if( !p.isEmpty() && !q.isEmpty() ){

		utility::removeFavoriteEntry( QString( "%1\t%2" ).arg( p,q ) ) ;

		tablewidget::deleteRow( table,row ) ;
	}

	table->setEnabled( true ) ;
}

void favorites::cancel()
{
	this->HideUI() ;
}

void favorites::add()
{
	DialogMsg msg( this ) ;

	auto dev = m_ui->lineEditDeviceAddress->text() ;
	auto m_path = m_ui->lineEditMountPath->text() ;

	if( dev.isEmpty() ){

		return msg.ShowUIOK( tr( "ERROR!" ),tr( "Device address field is empty" ) ) ;
	}
	if( m_path.isEmpty() ){

		return msg.ShowUIOK( tr( "ERROR!" ),tr( "Mount point path field is empty" ) ) ;
	}

	m_ui->tableWidget->setEnabled( false ) ;

	this->addEntries( { dev,m_path } ) ;

	utility::addToFavorite( dev,m_path ) ;

	m_ui->lineEditDeviceAddress->clear() ; ;
	m_ui->lineEditMountPath->clear() ;

	m_ui->tableWidget->setEnabled( true ) ;
}

void favorites::folderAddress()
{
	auto e = QFileDialog::getExistingDirectory( this,tr( "Path To An Encrypted Volume" ),
						    QDir::homePath(),QFileDialog::ShowDirsOnly ) ;

	while( true ){

		if( e.endsWith( '/' ) ){

			e.truncate( e.length() - 1 ) ;
		}else{
			break ;
		}
	}

	if( !e.isEmpty() ){

		m_ui->lineEditDeviceAddress->setText( [ & ]{

			while( true ){

				if( e.endsWith( '/' ) ){

					e.truncate( e.length() - 1 ) ;
				}else{
					break ;
				}
			}

			return e ;

		}() ) ;
	}
}

void favorites::fileAddress()
{
	auto e = QFileDialog::getOpenFileName( this,tr( "Path To An Encrypted Volume" ),QDir::homePath() ) ;

	if( !e.isEmpty() ){

		m_ui->lineEditDeviceAddress->setText( e ) ;
	}
}

favorites::~favorites()
{
	delete m_ui ;
}

void favorites::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->HideUI() ;
}

void favorites::currentItemChanged( QTableWidgetItem * current, QTableWidgetItem * previous )
{
	tablewidget::selectRow( current,previous ) ;
}
