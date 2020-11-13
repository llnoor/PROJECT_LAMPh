#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25
# Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

include( ../../common.pri )
include( ../../lib.pri )

TARGET = COM_SR850$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += COM_SR850.cpp

HEADERS += COM_SR850.h\
        COM_SR850_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
