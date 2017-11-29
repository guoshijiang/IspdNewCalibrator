#-------------------------------------------------
#
# Project created by QtCreator 2017-11-17T10:12:00
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IspdNewCalibrator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    toolbar.cpp \
    version.cpp \
    common.cpp \
    question.cpp \
    document.cpp \
    serialportset.cpp \
    handlepath.cpp \
    pugixml.cpp \
    parsexml.cpp \
    serialport.cpp \
    baseui.cpp \
    protoserialport.cpp \
    dbinfo.cpp \
    encrypt.cpp

HEADERS  += mainwindow.h \
    toolbar.h \
    version.h \
    common.h \
    question.h \
    document.h \
    serialportset.h \
    handlepath.h \
    pugiconfig.hpp \
    pugixml.hpp \
    parsexml.h \
    serialport.h \
    protoserialport.h \
    dbinfo.h \
    encrypt.h \
    baseui.hpp

FORMS    += mainwindow.ui \
    version.ui \
    question.ui \
    document.ui \
    serialportset.ui

RESOURCES += \
    resource.qrc
