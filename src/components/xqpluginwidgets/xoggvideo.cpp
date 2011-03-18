/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "xoggvideo.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* Phonon */
#include <Phonon/MediaSource>
#include <Phonon/BackendCapabilities>
#include <Phonon/ObjectDescriptionModel>
#include <Phonon/VideoPlayer>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

XOggVideo::XOggVideo ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "XOggVideo" ) );
  setWindowTitle ( i18n ( "Video Player" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setSpacing ( 0 );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  // Phonon::MediaObject
  m_mediaObject = new Phonon::MediaObject ( this );
  m_mediaObject->setTickInterval ( 1000 );

  // Phonon::VideoWidget
  m_videoWidget = new Phonon::VideoWidget ( this );
  Phonon::createPath ( m_mediaObject, m_videoWidget );
  vLayout->addWidget ( m_videoWidget );

  // Phonon::AudioOutput
  m_audioOutput = new Phonon::AudioOutput ( Phonon::VideoCategory, this );
  Phonon::createPath ( m_mediaObject, m_audioOutput );

  // Phonon::VolumeSlider
  m_volumeSlider = new Phonon::VolumeSlider ( this );
  m_volumeSlider->setAudioOutput ( m_audioOutput );
  vLayout->addWidget ( m_volumeSlider );

  setLayout ( vLayout );

  connect ( m_videoWidget, SIGNAL ( finished() ), m_videoWidget, SLOT ( deleteLater() ) );
}

void XOggVideo::setUrl ( const QUrl &url )
{
  qDebug() << Q_FUNC_INFO << url;
  Phonon::MediaSource source ( url );
  if ( source.type() == Phonon::MediaSource::Invalid )
  {
    qWarning ( "(XHTMLDBG) Can not read Video Source %s", qPrintable ( url.toString() ) );
    return;
  }

  m_mediaObject->setCurrentSource ( source );
}

const QString XOggVideo::getUrl () const
{
  return QString::null;
}

QString XOggVideo::name () const
{
  return QString::fromUtf8 ( "oggvideo" );
}

QString XOggVideo::description () const
{
  return i18n ( "Play OGG/OGM and Therora Videos with Phonon" );
}

QStringList XOggVideo::mimeTypes() const
{
  QStringList l;
  // file:///usr/share/mime/video/ogg.xml
  l << "application/ogg:ogv:Ogg Video";
  l << "video/x-ogg:ogv:Ogg Video";
  // file:///usr/share/mime/video/x-ogm+ogg.xml
  l << "video/x-ogm:ogm:OGM Video";
  // file:///usr/share/mime/video/x-theora+ogg.xml
  l << "video/ogg:ogg:Ogg Theora Video";
  l << "video/x-theora:ogg:Ogg Theora Video";
  return l;
}

XOggVideo::~XOggVideo()
{}
