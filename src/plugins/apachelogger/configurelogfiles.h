/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#ifndef CONFIGURELOGFILES_H
#define CONFIGURELOGFILES_H

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

/* KDE */
#include <KDE/KDialog>

class ConfigureLogFiles : public KDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QTreeWidget* m_treeWidget;
    QLineEdit* m_lineEdit;
    const QStringList directories();
    bool isScanDirectory ( const QString & );
    void toggleScanDirectory ( const QString &path );
    QTreeWidgetItem* logDirItem ( const QString & );
    void insertDirItem ( const QString &, bool checked = false );
    void insertFileItems ( QTreeWidgetItem* parent, const QFileInfo & ) const;
    void setChecked ( const QString & );

  private Q_SLOTS:
    void readConfiguration();
    void writeConfiguration();
    void openLogFileDialog();
    void insertClicked();
    void removeItemClicked();
    void itemChecked ( QTreeWidgetItem *, int );

  public:
    ConfigureLogFiles ( QWidget * parent = 0 );
    const QStringList logFiles ( const QString &path );
    ~ConfigureLogFiles();
};

#endif
