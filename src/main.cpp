#include "sizer.h"
#include <QGuiApplication>
#include <QDebug>

using namespace smuse;

int
main (int argc, char*argv[])
{
  QGuiApplication app;

  qDebug() << Q_FUNC_INFO;

  Sizer sizer;

  return app.exec();
}
