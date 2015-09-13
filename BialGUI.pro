#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T11:52:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BialGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlswidget.cpp \
    imageviewer.cpp \
    thumbswidget.cpp \
    imagewidget.cpp

HEADERS  += mainwindow.h \
    controlswidget.h \
    imageviewer.h \
    thumbswidget.h \
    imagewidget.h \
    viewerinterface.h

FORMS    += mainwindow.ui \
    controlswidget.ui \
    thumbswidget.ui \
    imagewidget.ui

RESOURCES += \
    qrs/resources.qrc

CONFIG += c++11
