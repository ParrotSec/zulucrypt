/*
 *
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

#include "tablewidget.h"
#include <QDebug>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>

#include <functional>

using count_t = decltype( QTableWidget().rowCount() ) ;

using function_t = std::function< void( count_t,count_t ) > ;

static void _for_each_column( QTableWidget * table,count_t row,function_t function )
{
	if( row >= 0 && row < table->rowCount() ){

		count_t col = table->columnCount() ;

		for( count_t i = 0 ; i < col ; i++ ){

			function( row,i ) ;
		}

		table->setCurrentCell( row,col - 1 ) ;
	}
}

static void _for_each_row( QTableWidget * table,count_t col,function_t function )
{
	if( col >= 0 && col < table->columnCount() ){

		count_t row = table->rowCount() ;

		for( count_t i = 0 ; i < row ; i++ ){

			function( i,col ) ;
		}
	}
}

static void _update_table_row( QTableWidgetItem * item,bool setSelected )
{
	if( item ){

		auto table = item->tableWidget() ;

		count_t row = item->row() ;
		count_t col = table->columnCount() ;

		for( count_t i = 0 ; i < col ; i++ ){

			table->item( row,i )->setSelected( setSelected ) ;
		}

		if( setSelected ){

			table->setCurrentCell( row,col - 1 ) ;
		}

		table->setFocus() ;
	}
}

static QTableWidgetItem * _set_item( QTableWidgetItem * item,
				     const QString& text = QString(),
				     const QFont& font = QFont() )
{
	item->setText( text ) ;
	item->setTextAlignment( Qt::AlignCenter ) ;
	item->setFont( font ) ;
	return item ;
}

void tablewidget::selectTableRow( QTableWidgetItem * current,QTableWidgetItem * previous )
{
	if( current && previous && previous->row() == current->row() ){

		auto table = current->tableWidget() ;

		table->setCurrentCell( current->row(),table->columnCount() - 1 ) ;
		table->setFocus() ;
	}else{
		_update_table_row( current,true ) ;
		_update_table_row( previous,false ) ;
	}
}

int tablewidget::addEmptyRow( QTableWidget * table )
{
	count_t row = table->rowCount() ;

	table->insertRow( row ) ;

	_for_each_column( table,row,[ table ]( count_t row,count_t col ){

		table->setItem( row,col,_set_item( new QTableWidgetItem ) ) ;
	} ) ;

	return row ;
}

int tablewidget::columnHasEntry( QTableWidget * table,const QString& entry,int column )
{
	if( column < 0 || column >= table->columnCount() ){

		return -1 ;
	}else{
		count_t rows = table->rowCount() ;

		for( count_t i = 0 ; i < rows ; i++ ){

			if( table->item( i,column )->text() == entry ){

				return i ;
			}
		}

		return -1 ;
	}
}

void tablewidget::addRowToTable( QTableWidget * table,const QStringList& list,const QFont& font )
{
	count_t j = list.size() ;

	if( j != table->columnCount() ){

		qDebug() << "ERROR: Table column count is NOT the same as QStringList size" ;
	}else{
		count_t row = table->rowCount() ;

		table->insertRow( row ) ;

		_for_each_column( table,row,[ & ]( count_t row,count_t col ){

			table->setItem( row,col,_set_item( new QTableWidgetItem,list.at( col ),font ) ) ;
		} ) ;
	}
}

void tablewidget::updateRowInTable( QTableWidget * table,const QStringList& list,int row,const QFont& font )
{
	count_t j = list.size() ;

	if( j != table->columnCount() ){

		qDebug() << "ERROR: table column count is NOT the same as QStringList size" ;
	}else{
		_for_each_column( table,row,[ & ]( count_t row,count_t col ){

			_set_item( table->item( row,col ),list.at( col ),font ) ;
		} ) ;
	}
}

void tablewidget::setRowFont( QTableWidget * table ,int row,const QFont& font )
{
	_for_each_column( table,row,[ & ]( count_t row,count_t col ){

		table->item( row,col )->setFont( font ) ;
	} ) ;
}

void tablewidget::deleteRowFromTable( QTableWidget * table,int row )
{
	if( row >= 0 && row < table->rowCount() ){

		table->removeRow( row ) ;

		tablewidget::selectLastRow( table ) ;
	}
}

void tablewidget::deleteTableRow( QTableWidget * table,const QString& value,int column )
{
	tablewidget::deleteRowFromTable( table,tablewidget::columnHasEntry( table,value,column ) ) ;
}

void tablewidget::selectRow( QTableWidget * table,int row )
{
	if( row >= 0 && row < table->rowCount() ){

		table->setCurrentCell( row,table->columnCount() - 1 ) ;
	}

	table->setFocus() ;
}

void tablewidget::selectRow( QTableWidget * table,const QString& e )
{
	tablewidget::selectRow( table,tablewidget::columnHasEntry( table,e ) ) ;
}

void tablewidget::selectLastRow( QTableWidget * table )
{
	tablewidget::selectRow( table,table->rowCount() - 1 ) ;
}

void tablewidget::setText( QTableWidget * table,int row,int col,const QString& text )
{
	table->setItem( row,col,_set_item( new QTableWidgetItem,text,table->item( row,col )->font() ) ) ;
}

QStringList tablewidget::tableColumnEntries( QTableWidget * table,int col )
{
	QStringList l ;

	_for_each_row( table,col,[ & ]( count_t row,count_t col ){

		l.append( table->item( row,col )->text() ) ;
	} ) ;

	return l ;
}

QStringList tablewidget::tableRowEntries( QTableWidget * table,int row )
{
	QStringList l ;

	_for_each_column( table,row,[ & ]( count_t row,count_t col ){

		l.append( table->item( row,col )->text() ) ;
	} ) ;

	return l ;
}

void tablewidget::clearTable( QTableWidget * table )
{
	auto j = table->rowCount() ;

	for( decltype( j ) i = 0 ; i < j ; i++ ){

		table->removeRow( 0 ) ;
	}
}
