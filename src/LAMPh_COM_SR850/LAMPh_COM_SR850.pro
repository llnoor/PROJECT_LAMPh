#-------------------------------------------------
#	This library is written to work with the lock-in SR850.
#	Created by Ilnur Gimazov 2018-10-17 (17th October 2018)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_SR850$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_SR850.cpp

HEADERS += LAMPh_COM_SR850.h\
        LAMPh_COM_SR850_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
