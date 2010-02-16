/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef SOURCEVIEW_H
#define SOURCEVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QListWidgetItem>

class Highlighter;

class SourceView : public QTextEdit
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qtidy.hjcms.de" )

  private:
    Highlighter *m_highlighter;
    QString tidyrcFile;

  private Q_SLOTS:
    void documentLines();
    void updateSelection();

  public Q_SLOTS:
    void fetchRow ( QListWidgetItem * );
    void setSource ( const QString & );
    void saveContent ( const QUrl & );

  public:
    SourceView ( QWidget *parent = 0 );
    const QString source();
    ~SourceView();
};

#endif
