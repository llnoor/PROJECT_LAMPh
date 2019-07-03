#-------------------------------------------------
#	This library was written to work with the multimeter KEITHLEY2000
#	Created by Ilnur Gimazov 2019-07-02 (2nd July 2019)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_KEITHLEY2000$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_KEITHLEY2000.cpp

HEADERS += LAMPh_COM_KEITHLEY2000.h\
        LAMPh_COM_KEITHLEY2000_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
