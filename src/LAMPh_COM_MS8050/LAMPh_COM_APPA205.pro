#-------------------------------------------------
#	To work with APPA 205
#	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2018-10-17 (17th October 2018)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_APPA205$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_APPA205.cpp

HEADERS += LAMPh_COM_APPA205.h\
        LAMPh_COM_APPA205_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
