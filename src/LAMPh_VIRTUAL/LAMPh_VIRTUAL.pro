#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_VIRTUAL$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_VIRTUAL.cpp

HEADERS += LAMPh_VIRTUAL.h\
        LAMPh_VIRTUAL_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
