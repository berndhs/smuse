#ifndef SIZER_H
#define SIZER_H


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2017, Bernd Stramm
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

#include <stddef.h>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QMap>
#include <QMimeDatabase>
#include <QTextStream>

namespace smuse {
class Sizer : public QObject
{
  Q_OBJECT
public:
  explicit Sizer(QObject *parent = 0);

  void setAllocSize (const size_t size) { m_allocSize = size; }
  size_t allocSize () { return m_allocSize; }

  void setListFile (const QString & file) { m_fileListFile = file; }
  QString listFile() { return m_fileListFile; }

  void setOutput (const QString & outfile) { m_output = outfile; }
  QString output () { return m_output; }

  void reportSpace();
signals:

public slots:

private:

  void reportTotals ();
  void report(QTextStream&output);

  class MimeType {
  public:
    MimeType();
    MimeType(QString n);
    QString name;
    size_t  totalSize;
    size_t  totalFrag;
    size_t  numFiles;
    double  avgSize;
  };

  QString       m_fileListFile;
  QString       m_output;
  size_t        m_allocSize;
  QMap<QString, MimeType>   m_mimeRecs;
  QMimeDatabase m_mimes;

  size_t      m_totalBytes;
  size_t      m_totalFrag;
  size_t      m_totalFiles;

};

} // namespace

#endif // SIZER_H
