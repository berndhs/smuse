
TEMPLATE = app
TARGET = sizer
QMAKE_CXX = g++
CONFIG += c++11

MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
OBJECTS_DIR = tmp/obj

HEADERS += \
    src/sizer.h \
    src/cmdoptions.h \
    src/messagehandler.h \
    src/gpl2.h

SOURCES += \
    src/sizer.cpp \
    src/main.cpp \
    src/cmdoptions.cpp \
    src/messagehandler.cpp

