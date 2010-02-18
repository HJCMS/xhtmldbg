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
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QListWidget>
#include <QtGui/QFont>
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
    Highlighter* m_highlighter;
    QListWidget* m_listWidget;

  private Q_SLOTS:
    void setLines();
    void updateSelection();

  Q_SIGNALS:
    void linesChanged ( const QList<QListWidgetItem*> & );

  public Q_SLOTS:
    void fetchRow ( QListWidgetItem * );
    void setSource ( const QString & );

  public:
    SourceView ( const QFont &font, QWidget *parent = 0 );
    const QString source();
    ~SourceView();

};

#endif
