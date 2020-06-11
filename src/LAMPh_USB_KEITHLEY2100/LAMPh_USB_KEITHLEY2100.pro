#-------------------------------------------------
#	This library was written to work with the multimeter KEITHLEY2100
#	Created by Ilnur Gimazov 2020-06-11 (11 June 2020)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_USB_KEITHLEY2100$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_USB_KEITHLEY2100.cpp

HEADERS += LAMPh_USB_KEITHLEY2100.h\
        LAMPh_USB_KEITHLEY2100_global.h\
        VISA/WinNT/include/visa.h\
        VISA/WinNT/include/visatype.h\

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$PWD/VISA/WinNT/lib/msc/ -lvisa32
LIBS += -L$$PWD/VISA/WinNT/lib/msc/ -livi
