#-------------------------------------------------
#
# Project created by QtCreator 2018-07-17
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11


include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_ATIME$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_ATIME.cpp

HEADERS += LAMPh_ATIME.h\
        LAMPh_ATIME_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
