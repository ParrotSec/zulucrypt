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

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QString>
#include <QStringList>
#include <QFont>

class QTableWidget ;
class QTableWidgetItem ;

namespace tablewidget
{
	void selectTableRow( QTableWidgetItem * current,QTableWidgetItem * previous ) ;
	void addRowToTable( QTableWidget *,const QStringList&,const QFont& = QFont() ) ;
	void updateRowInTable( QTableWidget *,const QStringList&,int row,const QFont& ) ;
	void setRowFont( QTableWidget *,int row,const QFont& ) ;
	void deleteRowFromTable( QTableWidget *,int row ) ;
	void deleteTableRow( QTableWidget *,const QString&,int = 0 ) ;
	void selectRow( QTableWidget *,int row ) ;
	void selectRow( QTableWidget *,const QString& ) ;
	void selectLastRow( QTableWidget * ) ;
	void setText( QTableWidget *,int row,int col,const QString& text ) ;
	int addEmptyRow( QTableWidget * ) ;
	int columnHasEntry( QTableWidget *,const QString&,int = 0 ) ;
	QStringList tableColumnEntries( QTableWidget * table,int = 0 ) ;
	QStringList tableRowEntries( QTableWidget * table,int = 0 ) ;
	void clearTable( QTableWidget * ) ;
}

#endif // TABLEWIDGET_H
