/*
 *
 *  Copyright (c) 2015
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

#ifndef CHECKFORUPDATES_H
#define CHECKFORUPDATES_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

class QWidget ;
class QNetworkReply ;

#include <memory>

class QNetworkReply ;
class QWidget ;

class checkForUpdates : public QObject
{
	Q_OBJECT
public:
	checkForUpdates( QWidget *,bool ) ;
	static void instance( QWidget *,const QString& ) ;
	static void instance( QWidget * ) ;
public slots:
	void networkReply( QNetworkReply * ) ;
private:
	QWidget * m_widget ;
	bool m_autocheck ;
	QNetworkAccessManager m_manager ;
};

#endif // CHECKFORUPDATES_H
