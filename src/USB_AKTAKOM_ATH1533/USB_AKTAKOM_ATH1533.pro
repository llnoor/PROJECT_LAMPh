#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = USB_AKTAKOM_ATH1533$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += USB_AKTAKOM_ATH1533.cpp

HEADERS += USB_AKTAKOM_ATH1533.h\
        USB_AKTAKOM_ATH1533_global.h\
    ATH1535.h \
    AULNgdev.h \
    cvitype.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lATH1535
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lATH1535d
else:unix: LIBS += -L$$PWD/./ -lATH1535

LIBS += -L -lAULFlib


