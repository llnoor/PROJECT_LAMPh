#-------------------------------------------------
#	This library is written to work with the lock-in LockIn5209.
#	Created by Ilnur Gimazov 2019-09-02 (2nd September 2019)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_COM_LockIn5209$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_COM_LockIn5209.cpp

HEADERS += LAMPh_COM_LockIn5209.h\
        LAMPh_COM_LockIn5209_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
