#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T11:52:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BialGUI
TEMPLATE = app

include(../bial/bial.pri)

SOURCES += \
    controller.cpp \
    controlswidget.cpp \
    dicomdir.cpp \
    displayformat.cpp \
    graphicsscene.cpp \
    guiimage.cpp \
    imageviewer.cpp \
    imagewidget.cpp \
    main.cpp\
    mainwindow.cpp \
    pixmaplabelitem.cpp \
    thumbnail.cpp \
    thumbswidget.cpp \


HEADERS  += \
       controller.h \
    controlswidget.h \
    dicomdir.h \
    displayformat.h \
    graphicsscene.h \
    guiimage.h \
    imageviewer.h \
    imagewidget.h \
    mainwindow.h \
    pixmaplabelitem.h \
    thumbnail.hpp \
    thumbswidget.h \
    viewerinterface.h \


FORMS    += mainwindow.ui \
    controlswidget.ui \
    thumbswidget.ui \
    imagewidget.ui

RESOURCES += \
    qrs/resources.qrc

CONFIG += c++11
