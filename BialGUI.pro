#-------------------------------------------------
#
# Project created by QtCreator 2015-09-12T11:52:54
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BialGUI
TEMPLATE = app

include(../bial/bial.pri)
include(gdcm.pri)

SOURCES += \
    thirdParty/qcustomplot.cpp \
    tool.cpp \
    defaulttool.cpp

SOURCES += \
    controller.cpp \
    controlswidget.cpp \
    dicomdir.cpp \
    displayformat.cpp \
    gdcm.cpp \
    graphicsscene.cpp \
    guiimage.cpp \
    imageviewer.cpp \
    imagewidget.cpp \
    main.cpp\
    mainwindow.cpp \
    segmentationwidget.cpp \
    thumbnail.cpp \
    thumbswidget.cpp \

HEADERS += \
    thirdParty/qcustomplot.h \
    tool.h \
    defaulttool.h

HEADERS  += \
    controller.h \
    controlswidget.h \
    dicomdir.h \
    displayformat.h \
    gdcm.h \
    graphicsscene.h \
    guiimage.h \
    imageviewer.h \
    imagewidget.h \
    mainwindow.h \
    segmentationwidget.h \
    thumbnail.hpp \
    thumbswidget.h \
    viewerinterface.h \


FORMS    += \
    controlswidget.ui \
    imagewidget.ui \
    mainwindow.ui \
    segmentationwidget.ui \
    thumbswidget.ui \

RESOURCES += \
    qrs/resources.qrc

CONFIG += c++11
