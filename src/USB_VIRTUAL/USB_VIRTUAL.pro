#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

include( ../../common.pri )
include( ../../lib.pri )

TARGET = USB_VIRTUAL$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += USB_VIRTUAL.cpp

HEADERS += USB_VIRTUAL.h\
        USB_VIRTUAL_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
