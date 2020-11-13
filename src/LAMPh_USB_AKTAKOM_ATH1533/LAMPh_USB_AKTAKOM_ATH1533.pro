#-------------------------------------------------
#	This library is written to work with the .
#	Created by Ilnur Gimazov 2018-10-23 (23th October 2018)
#	Lib for LAMPh
#
#-------------------------------------------------

QT       -= gui
QT += serialport

CONFIG += C++11

include( ../../common.pri )
include( ../../lib.pri )

TARGET = LAMPh_USB_AKTAKOM_ATH1533$${LIB_SUFFIX}
TEMPLATE = lib

DEFINES += LAMPHLIB_LIBRARY

SOURCES += LAMPh_USB_AKTAKOM_ATH1533.cpp

HEADERS += LAMPh_USB_AKTAKOM_ATH1533.h\
        LAMPh_USB_AKTAKOM_ATH1533_global.h\
    ATH1535.h \
    AULNgdev.h \
    cvitype.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lATH1535
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lATH1535d
else:unix: LIBS += -L$$PWD/./ -lATH1535

LIBS += -L -lAULFlib
