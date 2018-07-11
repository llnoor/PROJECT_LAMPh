include( ../../common.pri )
include( ../../app.pri )

TARGET = LAMPhDevices
TEMPLATE = app

QT += widgets
QT += serialport

CONFIG += C++11

HEADERS       = window.h \
    logowindow.h \
    pixmaps.h \
    lamphdevices.h
SOURCES       = window.cpp \
                main.cpp \
    logowindow.cpp \
    lamphdevices.cpp



