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


#ifndef HMAC_H
#define HMAC_H

#include <QDialog>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QWidget>

#include <functional>

class QCloseEvent ;
class QEvent ;

#include "../plugins/plugins.h"

namespace Ui {
class plugin;
}

class plugin : public QDialog
{
	Q_OBJECT
public:
	static plugin& instance( QWidget * parent,
				  plugins::plugin t,
				  std::function< void( const QByteArray& ) > e,
				  const QString& f = QString(),
				  const QVector<QString>& g = QVector<QString>() )
	{
		return *( new plugin( parent,t,e,f,g ) ) ;
	}
	plugin( QWidget * parent,
	      plugins::plugin,
	      std::function< void( const QByteArray& ) >,
	      const QString& = QString(),const QVector<QString>& = QVector<QString>() ) ;
	~plugin() ;
private slots:
	void pbSetKey() ;
	void pbSelectKeyFile() ;
	void pbClose() ;
private:
	void ShowUI() ;
	void HideUI() ;
	void enableAll() ;
	void disableAll() ;
	void closeEvent( QCloseEvent * ) ;
	bool eventFilter( QObject * watched,QEvent * event ) ;
	Ui::plugin * m_ui ;
	QByteArray m_key = QByteArray() ;
	std::function< void( const QByteArray& ) > m_function ;
	plugins::plugin m_pluginType ;
	QVector<QString> m_exe ;
};

#endif // HMAC_H
