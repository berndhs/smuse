#include "sizer.h"
#include <QGuiApplication>
#include <QDebug>
#include "cmdoptions.h"

using namespace smuse;

int
main (int argc, char*argv[])
{
//  QGuiApplication app(argc,argv);

  deliberate::CmdOptions opts("Sizer");
  bool versionSeen(false);
  opts.SetSoloOpt("version",versionSeen);
  opts.Parse(argc,argv);

  if (opts.SeenOpt("version")) {
    return 1;
  }
  qDebug() << Q_FUNC_INFO;

  Sizer sizer;

  sizer.setListFile(argv[1]);
  sizer.reportSpace();
  qDebug() << "alloc size was " << sizer.allocSize();
  qDebug() << "file list in " << sizer.listFile();

//  return app.exec();
}
