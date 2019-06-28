#-------------------------------------------------
#	This is a template for writing a new library for LAMPh
#	Created by Ilnur Gimazov 2019-06-25 (25th June 2019)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_MS8050$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_MS8050.cpp

HEADERS += LAMPh_COM_MS8050.h\
        LAMPh_COM_MS8050_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
