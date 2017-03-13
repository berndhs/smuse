#include "sizer.h"


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

#include <QFile>
#include <QFileInfo>
#include <QMimeType>
#include <QMimeDatabase>
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QStringList>
#include <stdio.h>
#include <climits>
#include <unistd.h>

namespace smuse {

Sizer::Sizer(QObject *parent) : QObject(parent),
  m_fileListFile(QString()),
  m_output(QString()),
  m_allocSize(4096),
  m_modeOfOperation(Mode::DontKnow)
{
  qDebug() << Q_FUNC_INFO;
  qDebug() << Q_FUNC_INFO << QDir::current();
}

void
Sizer::setStartDir(const QStringList &list)
{
  qDebug() << Q_FUNC_INFO << list;
  m_startList = list;
  m_modeOfOperation = Mode::StartDir;
}

void
Sizer::start()
{
  qDebug() << Q_FUNC_INFO ;
  qDebug() << Q_FUNC_INFO << QDir::current();
  if (m_modeOfOperation == Mode::StartDir) {
    restart(m_startList);
  } else {
    runFileList();
  }
  reportTotals();
}

void Sizer::examineFile(QString &daFile)
{
  qDebug() << Q_FUNC_INFO << daFile;
  qDebug() << Q_FUNC_INFO << QDir::current();
  QFileInfo info (daFile.trimmed());
  QFile fl(daFile.trimmed());
  size_t thisSize = fl.size();
  size_t thisFrag = frag(thisSize);
  qDebug() << Q_FUNC_INFO << thisSize << thisFrag;
  QString mimeName = m_mimes.mimeTypeForFile(info).name();
  MimeType mime = m_mimeRecs[mimeName];
  mime.name = mimeName;
  mime.numFiles ++;
  mime.totalSize += thisSize;
  mime.totalFrag += thisFrag;
  mime.avgSize = mime.totalSize / mime.numFiles;
  m_mimeRecs[mimeName] = mime;
  qDebug() << Q_FUNC_INFO << daFile << mime;
}

void
Sizer::runFileList()
{
  qDebug() << Q_FUNC_INFO ;
  qDebug() << Q_FUNC_INFO << QDir::current();
  QFile file (m_fileListFile);
  bool ok = file.open(QFile::ReadOnly);
  if (ok) {
    QStringList bunch;
    while (!file.atEnd()) {
      int i;
      for (i=0; i<100 && !file.atEnd(); ++i) {
        QByteArray name = file.readLine(INT_MAX);
        bunch << QString(name);
      }
      restart(bunch);
      bunch.clear();
    }
  }
}


void
Sizer::runDir(const QString &dirName)
{
  QDir dir(dirName);
  qDebug() << "runDir a" << QDir::current();
  QStringList files = dir.entryList();
  QDir home = QDir::current();
  chdir(dirName.toStdString().c_str());
  qDebug() << "runDir b" <<  QDir::current();
  files.removeAll(".");
  files.removeAll("..");
  qDebug() << Q_FUNC_INFO << " files: " << files;
  restart (files);
  chdir(home.absolutePath().toStdString().c_str());
  qDebug() << "runDir c" <<  QDir::current();
}

void
Sizer::restart(const QStringList &nameList)
{
  qDebug() << Q_FUNC_INFO << nameList;
  QStringList namesCopy (nameList);
  while (!namesCopy.isEmpty()) {
    QString nextName = namesCopy.takeFirst();
    QFileInfo info(nextName);
    QFileInfo info2 (info.canonicalFilePath());
    qDebug() << Q_FUNC_INFO << QDir::current();
    if (info2.isDir()) {
      runDir(nextName);
    } else {
      examineFile(nextName);
    }
  }
}

void
Sizer::reportTotals()
{
  qDebug() << Q_FUNC_INFO ;
  if(m_output.isEmpty()) {
    QTextStream standard (stdout);
    report(standard);
  } else {
    QFile out(m_output);
    out.open(QFile::WriteOnly);
    QTextStream outstream(&out);
    report (outstream);
    out.close();
  }
}


void
Sizer::report(QTextStream &output)
{
  output << "-------------------Report-----------------------" << endl;
  if (!m_fileListFile.isEmpty()) {
    output << "report on === " << m_fileListFile
           << " ===  "<<m_totalFiles << " files" << endl;
  } else if (!m_startList.isEmpty()) {
    output << "report on === " ;
    for (auto s= m_startList.begin(); s!= m_startList.end(); ++s)
      output << *s << "\t";
    output << endl;
  }
  for (auto m=m_mimeRecs.begin(); m!= m_mimeRecs.end(); ++m) {
    output << "\ntype \"" << m->name << "\" total size " << m->totalSize <<
                 " avg Size " << m->avgSize << " number " << m->numFiles<< endl;
    output << "fragmentation internal: " << double(m->totalFrag)/(double(m->numFiles)*m->avgSize) * 100.0
           << " % of gross filesize" << endl;
    double sz = double(m->totalSize);
    output <<"\t" << sz << " size bytes "
               << double(sz)/1024.0 << " KiB "
               << double(sz)/(1024.0*1024.0) << " MiB "
               << double(sz)/(1024.0*1024.0*1024.0) << " GiB"<< endl;
    sz = double(m->totalFrag);
    output <<"\t" << sz << " frag bytes "
               << double(sz)/1024.0 << " KiB "
               << double(sz)/(1024.0*1024.0) << " MiB "
               << double(sz)/(1024.0*1024.0*1024.0) << " GiB"<< endl;

  }

}

size_t Sizer::frag(size_t bytes)
{
  return m_allocSize - (bytes % m_allocSize);
}

Sizer::MimeType::MimeType()
  :MimeType(QString())
{}

Sizer::MimeType::MimeType(QString n)
{
  name = n;
  totalSize = 0;
  numFiles = 0;
  totalFrag = 0;
  avgSize = 0.9;
}

} // namespace
