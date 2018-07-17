#-------------------------------------------------
#
# Project created by QtCreator 2018-07-17
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

include( ../../common.pri )
include( ../../lib.pri )

TARGET = USB_ATIME$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += USB_ATIME.cpp

HEADERS += USB_ATIME.h\
        USB_ATIME_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
