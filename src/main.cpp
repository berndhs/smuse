#include "sizer.h"
#include <QGuiApplication>
#include <QDebug>
#include "cmdoptions.h"
#include <QtGlobal>
#include <QStringList>
#include "messagehandler.h"

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

using namespace smuse;

int
main (int argc, char*argv[])
{
//  QGuiApplication app(argc,argv);

  qInstallMessageHandler(deliberate::MessageHandler);
  deliberate::CmdOptions opts("sizer");
  opts.AddStringOption("output","O",QObject::tr("write output to a file"));
  opts.AddSoloOption ("debug","D",QObject::tr("show debug messages"));
  opts.AddSoloOption("quiet","Q",QObject::tr("supress debug messages, be extra quiet"));
  opts.AddStringOption ("logdebug","L",QObject::tr("write Debug log to file"));
  opts.AddStringOption("file","F",QObject::tr("list of files in this file"));

  opts.addUsage(QObject::tr("[dir] - start with directory dir, otherwise start with current"));

  bool versionSeen(false);
  opts.SetSoloOpt("version",versionSeen);
  bool optsok = opts.Parse(argc,argv);
  if (!optsok) {
    opts.Usage();
    exit(1);
  }
  deliberate::showDebug = false;
  if (opts.SeenOpt("debug") && !opts.SeenOpt("quiet")) {
    deliberate::showDebug = true;
  }

  if (opts.SeenOpt("version")) {
    return 1;
  }
  if (opts.SeenOpt("help")) {
    opts.Usage();
    return 2;
  }

  Sizer sizer;
  if (opts.SeenOpt("output")) {
    sizer.setOutput(opts.ValueList("output").takeFirst().toString());
  }

  if (opts.SeenOpt("file")) {
    sizer.setListFile(opts.ValueList("file").takeFirst().toString());
  } else {
    if (argc >= 1) {
      QStringList args ;
      for (int i=1;i<argc;++i) {
        args << argv[i];
      }
      sizer.setStartDir(args);
    } else {
      QStringList args ;
      args << ".";
      sizer.setStartDir(args);
    }
  }
  qDebug() << Q_FUNC_INFO << "about to start";
  sizer.start();
  qDebug() << Q_FUNC_INFO << "back from start";
  qDebug() << "alloc size was " << sizer.allocSize();

//  return app.exec();
}
