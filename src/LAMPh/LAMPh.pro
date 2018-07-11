include( qwtWidgetLibrary.pri )
include( ../../common.pri )
include( ../../app.pri )

TARGET = LAMPh
TEMPLATE = app

QT += widgets
QT += sql
QT += serialport

CONFIG += C++11

HEADERS       = window.h \
    logowindow.h \
    authorization.h \
    sqlamp.h \
    initdb.h \
    experiments.h \
    database.h \
    dialogauth.h \
    dialogreg.h \
    dialogexp.h \
    lamphplot.h \
    incrementalplot.h \
    mainplot.h \
    pixmaps.h \
    lamphsetting.h \
    lamphedit.h \
    datatable.h \
    lamphdevices.h \
    lamphtemp.h
SOURCES       = window.cpp \
                main.cpp \
    logowindow.cpp \
    authorization.cpp \
    sqlamp.cpp \
    experiments.cpp \
    database.cpp \
    dialogauth.cpp \
    dialogreg.cpp \
    dialogexp.cpp \
    lamphplot.cpp \
    incrementalplot.cpp \
    mainplot.cpp \
    lamphsetting.cpp \
    lamphedit.cpp \
    datatable.cpp \
    lamphdevices.cpp \
    lamphtemp.cpp


DISTFILES += \
    conf.txt \
    README.md \
    photo1.jpg \
    photo_sql.jpg

