#-------------------------------------------------
#
# Project created by QtCreator 2016-09-13T14:39:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GLOBALCONFIG.cpp \
    GLOBALFUNC.cpp \
    attrRecognize/waysInterface.cpp \
    GLOBALDEFINE.cpp \
    flagform.cpp \
    myimglabel.cpp \
    imgdetail.cpp \
    imgdata.cpp

HEADERS  += mainwindow.h \
    GLOBALDEFINE.h \
    GLOBALFUNC.h \
    GLOBALHEADER.h \
    GLOBALCONFIG.h \
    attrRecognize/waysInterface.h \
    flagform.h \
    myimglabel.h \
    imgdetail.h \
    imgdata.h

FORMS    += mainwindow.ui

RESOURCES += \
    files.qrc
