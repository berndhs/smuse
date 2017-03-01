
TEMPLATE = app
TARGET = sizer

CONFIG += c++11

MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
OBJECTS_DIR = tmp/obj

HEADERS += \
    src/sizer.h \
    src/cmdoptions.h

SOURCES += \
    src/sizer.cpp \
    src/main.cpp \
    src/cmdoptions.cpp

