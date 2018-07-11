#-------------------------------------------------
#
# Project created by QtCreator 2018-07-04
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

include( ../../common.pri )
include( ../../lib.pri )

TARGET = COM_VIRTUAL$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += COM_VIRTUAL.cpp

HEADERS += COM_VIRTUAL.h\
        COM_VIRTUAL_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
