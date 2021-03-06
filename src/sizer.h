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
#include <QStringList>
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

  void setStartDir (const QStringList & files) ;
  void setListFile (const QString & file) { m_fileListFile = file; }
  QString listFile() { return m_fileListFile; }

  void start();

  void setOutput (const QString & outfile) { m_output = outfile; }
  QString output () { return m_output; }

signals:

public slots:

private:

  enum class Mode {
    DontKnow = 0,
    StartDir = 1,
    PathFile
  };

  void examineFile (QString & daFile);
  void runFileList();
  void runDir(const QString & dirName);
  void restart (const QStringList & nameList);  // process a bunch of files
  void reportTotals ();
  void report(QTextStream&output);

  size_t frag (size_t bytes);

  class MimeType {
  public:
    MimeType();
    MimeType(QString n);
    operator QString () const {return QString("\nname: %1").arg(name)
          + QString ("\ntotalSize: %1").arg(totalSize)
          + QString("\ntotalFrag: %1").arg(totalFrag)
          + QString("\nnumFiles: %1").arg(numFiles)
          + QString("\navgSize: %1").arg(avgSize);
                        };
    QString name;
    size_t  totalSize;
    size_t  totalFrag;
    size_t  numFiles;
    double  avgSize;
  };

  QStringList   m_startList;
  QString       m_fileListFile;
  QString       m_output;
  size_t        m_allocSize;
  QMap<QString, MimeType>   m_mimeRecs;
  QMimeDatabase m_mimes;

  Mode        m_modeOfOperation;

  size_t      m_totalBytes;
  size_t      m_totalFrag;
  size_t      m_totalFiles;

};

} // namespace

#endif // SIZER_H
