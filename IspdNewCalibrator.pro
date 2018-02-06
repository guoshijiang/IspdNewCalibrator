#-------------------------------------------------
#
# Project created by QtCreator 2017-11-17T10:12:00
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += serialport
QT       += webkit webkitwidgets

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
    protoserialport.cpp \
    dbinfo.cpp \
    encrypt.cpp \
    controlserverstutasinfo.cpp \
    readspecificconfig.cpp \
    writespecconfig.cpp \
    restartmr.cpp \
    factoryreset.cpp \
    syncspecmrdomain.cpp \
    startupmrautodomain.cpp \
    startupmrautofactoryreset.cpp \
    basehttpclient.cpp \
    serialportwriteread.cpp \
    udpworker.cpp

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
    controlserverstutasinfo.h \
    readspecificconfig.h \
    writespecconfig.h \
    restartmr.h \
    factoryreset.h \
    syncspecmrdomain.h \
    startupmrautodomain.h \
    startupmrautofactoryreset.h \
    basehttpclient.h \
    serialportwriteread.h \
    udpworker.h


FORMS    += mainwindow.ui \
    version.ui \
    question.ui \
    document.ui \
    serialportset.ui \
    controlserverstutasinfo.ui \
    readspecificconfig.ui \
    writespecconfig.ui \
    restartmr.ui \
    factoryreset.ui \
    syncspecmrdomain.ui \
    startupmrautodomain.ui \
    startupmrautofactoryreset.ui \

RESOURCES += \
    resource.qrc

DISTFILES += \
    network
