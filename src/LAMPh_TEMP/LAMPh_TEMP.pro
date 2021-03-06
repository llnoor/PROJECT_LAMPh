#-------------------------------------------------
#	This LIB allows to convert physical quantities (thermal electromotive force, resistance, etc.) #   into temperature
#	Created by Ilnur Gimazov 2018-10-12 (12th October 2018)
#	Lib for LAMPh
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
