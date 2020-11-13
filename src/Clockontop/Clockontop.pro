
TARGET = LAMPhClockOnTop
TEMPLATE = app

QT += widgets

CONFIG += C++11

HEADERS       = window.h \
    logowindow.h 
	
SOURCES       = window.cpp \
                main.cpp \
    logowindow.cpp 

RC_FILE = logo.rc
