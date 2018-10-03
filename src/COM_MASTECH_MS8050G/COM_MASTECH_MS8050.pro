#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = COM_MASTECH_MS8050g$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += COM_MASTECH_MS8050.cpp

HEADERS += COM_MASTECH_MS8050.h\
        COM_MASTECH_MS8050_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
