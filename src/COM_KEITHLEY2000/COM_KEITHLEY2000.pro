#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = COM_KEITHLEY2000$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += COM_KEITHLEY2000.cpp

HEADERS += COM_KEITHLEY2000.h\
        COM_KEITHLEY2000_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
