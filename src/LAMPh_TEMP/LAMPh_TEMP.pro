#-------------------------------------------------
#	This is a template for writing a new library for LAMPh
#	Created by Ilnur Gimazov 2018-10-03 (3th October 2018)
#	Lib for LAMPh
#	Name of device (TEMP) must be changed everywhere. (LAMPh_TEMP -> LAMPh_NEW_DEVICE)
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_TEMP$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_TEMP.cpp

HEADERS += LAMPh_TEMP.h\
        LAMPh_TEMP_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
