/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#ifndef VALIDATOR_H
#define VALIDATOR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTemporaryFile>

class Validator : public QProcess
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( QString url READ getValidation WRITE setValidation )

  private:
    QTemporaryFile tempFile;
    QString errorLog;
    QStringList parameters;
    QString javaAppl;
    QString url;
    const QString param ( const QString &, QSettings * ) const;

  private Q_SLOTS:
    void cleaning();

  Q_SIGNALS:
    void criticalError ( const QString & );

  public Q_SLOTS:
    void validate ();

  public:
    Validator ( QObject * parent = 0 );
    bool isRunning();
    const QString getValidation ();
    bool setValidation ( const QString &url );
    void setEnviromentVariable ( QSettings * );
    const QString errorsLogFile();
    virtual ~Validator();
};

#endif
