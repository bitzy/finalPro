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
    attrRecognize/sleeveways.cpp \
    attrRecognize/stdconfig.cpp \
    attrRecognize/imgdata.cpp \
    attrRecognize/textureWays.cpp \
    attrRecognize/superpixeltool.cpp \
    attrRecognize/planimetry_tools.cpp \
    attrRecognize/skinDetect.cpp \
    attrRecognize/clothColor.cpp \
    attrRecognize/colorkmeans.cpp \
    attrRecognize/collarways.cpp \
    attrRecognize/uplenWays.cpp \
    attrRecognize/libsvm/svm.cpp \
    attrRecognize/svmpredict.cpp \
    runmanager.cpp \
    runthread.cpp \
    attrRecognize/tinyxml/tinystr.cpp \
    attrRecognize/tinyxml/tinyxml.cpp \
    attrRecognize/tinyxml/tinyxmlerror.cpp \
    attrRecognize/tinyxml/tinyxmlparser.cpp

HEADERS  += mainwindow.h \
    GLOBALDEFINE.h \
    GLOBALFUNC.h \
    GLOBALHEADER.h \
    GLOBALCONFIG.h \
    attrRecognize/waysInterface.h \
    flagform.h \
    myimglabel.h \
    attrRecognize/stdconfig.h \
    attrRecognize/imgdata.h \
    attrRecognize/superpixeltool.h \
    attrRecognize/planimetry_tools.h \
    attrRecognize/colorkmeans.h \
    attrRecognize/libsvm/svm.h \
    attrRecognize/svmpredict.h \
    runmanager.h \
    runthread.h \
    attrRecognize/tinyxml/tinystr.h \
    attrRecognize/tinyxml/tinyxml.h

FORMS    += mainwindow.ui

win32{
    INCLUDEPATH += C:\Users\sharon\Downloads\opencv\build\include \
        C:\Users\sharon\Downloads\opencv\build\include\opencv \
        C:\Users\sharon\Downloads\opencv\build\include\opencv2
    Debug: LIBS += -LC:\Users\sharon\Downloads\opencv\build\x64\vc12\lib \
        -lopencv_ts300d \
        -lopencv_world300d
    Release: LIBS += -L C:\Users\sharon\Downloads\opencv\build\x64\vc12\lib \
        -lopencv_ts300 \
        -lopencv_world300
}

unix{
    INCLUDEPATH += /usr/local/opencv3/include \
        /usr/local/opencv3/include/opencv \
        /usr/local/opencv3/include/opencv2
    LIBS += -L /usr/local/opencv3/lib/ \
         /usr/local/opencv3/lib/libopencv_highgui.so \
        /usr/local/opencv3/lib/libopencv_core.so \
        /usr/local/opencv3/lib/libopencv_imgproc.so \
        /usr/local/opencv3/lib/libopencv_imgcodecs.so \
        /usr/local/opencv3/lib/libopencv_ml.so \
        /usr/local/opencv3/lib/libopencv_objdetect.so \
        /usr/local/opencv3/lib/libopencv_ximgproc.so
}
