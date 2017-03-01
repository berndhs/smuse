#include "sizer.h"


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include <QFile>
#include <QFileInfo>
#include <QMimeType>
#include <QMimeDatabase>

namespace smuse {
Sizer::Sizer(QObject *parent) : QObject(parent),
  m_fileListFile(""),
  m_allocSize(4096)
{
  qDebug() << Q_FUNC_INFO;
}

void
reportK (size_t sz)
{
  qDebug() << sz << "bytes \n"
             << double(sz)/1024.0 << "KiB"
             << double(sz)/(1024.0*1024.0) << "MiB"
             << double(sz)/(1024.0*1024.0*1024.0) << "GiB";


}

void Sizer::reportSpace()
{
  qDebug() << Q_FUNC_INFO;
  QFile list (m_fileListFile);
  list.open(QFile::ReadOnly);
  size_t fragmentTotal(0);
  size_t fcount(0);
  size_t tsize(0);
  if (list.isOpen()) {
    while (!list.atEnd()) {
      QString nextName = QString(list.readLine().toStdString().c_str());

      qDebug() << "next file is " << nextName;
      QFileInfo info (nextName.trimmed());
      size_t thisSize = info.size();
      size_t thisFrag = thisSize % m_allocSize;
      QString mimeName = m_mimes.mimeTypeForFile(info).name();
      MimeType mime = m_mimeRecs[mimeName];
      mime.name = mimeName;
      mime.numFiles ++;
      mime.totalSize += thisSize;
      mime.avgSize = mime.totalSize / mime.numFiles;
      m_mimeRecs[mimeName] = mime;
      ++fcount;
      fragmentTotal += thisFrag;
      tsize += thisSize;
      qDebug() << "mime " << m_mimes.mimeTypeForFile(info);
      qDebug() << " last frag " <<thisFrag << " total " << fragmentTotal;
    }
  }
  qDebug() << "total files: " <<fcount
           << "\nsize: ";
  reportK(tsize);
  qDebug() << "fragementation:";
  reportK(fragmentTotal);
  qDebug () << "mime info\n" ;
  for (auto m=m_mimeRecs.begin(); m!= m_mimeRecs.end(); ++m) {
    qDebug () << "type " << m->name << "total size " << m->totalSize <<
                 "avg Size" << m->avgSize << "number " << m->numFiles;
  }
}

Sizer::MimeType::MimeType()
{
  name = QString();
  numFiles = 0;
  totalSize = 0;
  avgSize = 0.0;
}

Sizer::MimeType::MimeType(QString n)
{
  name = n;
  totalSize = 0;
  numFiles = 0;
  avgSize = 0.9;
}

} // namespace
