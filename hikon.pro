#-------------------------------------------------
#
# Project created by QtCreator 2023-07-15T15:18:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hikon
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    mycamera.cpp \
    grabimgthread.cpp \
    showimgthread.cpp \
    opencvtoqt.cpp \
    grab2imgthread.cpp

HEADERS += \
        widget.h \
    mycamera.h \
    grabimgthread.h \
    showimgthread.h \
    opencvtoqt.h \
    grab2imgthread.h

FORMS += \
        widget.ui

INCLUDEPATH +=D:\OpenCV-MinGW-Build-OpenCV-3.4.5\include\
              D:\OpenCV-MinGW-Build-OpenCV-3.4.5\include\opencv
              D:\OpenCV-MinGW-Build-OpenCV-3.4.5\include\opencv2
LIBS +=D:\OpenCV-MinGW-Build-OpenCV-3.4.5\x86\mingw\bin\libopencv_*.dll



INCLUDEPATH += $$PWD\includes

LIBS += -L$$PWD/libs/ -lMvCameraControl\
