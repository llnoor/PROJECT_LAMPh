#-------------------------------------------------
#	To work with RelayModule (ArduinoDUE)
#	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2019-06-26 (26th June 2019)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_RelayModule$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_RelayModule.cpp

HEADERS += LAMPh_COM_RelayModule.h\
        LAMPh_COM_RelayModule_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
