#-------------------------------------------------
#	Created by Ilnur Gimazov 2018-10-18 (18th October 2018)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_USB_AGILENT$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_USB_AGILENT.cpp

HEADERS += LAMPh_USB_AGILENT.h\
        LAMPh_USB_AGILENT_global.h\
        visa.h\
        visatype.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$PWD/./ -lagvisa32
LIBS += -L$$PWD/./ -lagvisaext
LIBS += -L$$PWD/./ -lvisa32
LIBS += -L$$PWD/./ -lvisaext

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lvisa32
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lvisa32d
#else:unix: LIBS += -L$$PWD/./ -lvisa32
# LIBS += -L -lvisa32
