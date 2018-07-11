#-------------------------------------------------
#
# Project created by QtCreator 2018-02-25
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

include( ../../common.pri )
include( ../../lib.pri )

TARGET = COM_APPA205$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += COM_APPA205.cpp

HEADERS += COM_APPA205.h\
        COM_APPA205_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
