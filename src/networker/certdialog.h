/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef CERTDIALOG_H
#define CERTDIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QSslCertificate>

class NetworkSettings;

class CertDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    NetworkSettings* m_networkSettings;
    QString certHost;
    QLabel* notify;
    QTreeWidget* treeWidget;
    QTreeWidgetItem* subjectInfo;
    QTreeWidgetItem* issuerInfo;
    struct RowItem
    {
      QString text;
      QString tip;
      QSslCertificate::SubjectInfo info;
    };
    QVector<RowItem> RowItems;

  private Q_SLOTS:
    void import();

  public:
    CertDialog ( NetworkSettings * settings, QWidget * parent = 0 );
    void setCertificate ( const QSslCertificate &, const QString & );
    void setMessages ( const QStringList & );
    ~CertDialog();
};

#endif
