#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T09:05:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spatialFilter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialoglog.cpp \
    mypixmapitem.cpp \
    imageprocessor.cpp \
    gaussblurdialog.cpp \
    biateraldialog.cpp \
    mediandialog.cpp \
    dilationdialog.cpp \
    erodedialog.cpp \
    fftToFill.cpp \
    affinedialog.cpp \
    stretchdialog.cpp \
    logpara.cpp \
    lpdialog.cpp \
    hpdialog.cpp

HEADERS += \
        mainwindow.h \
    dialoglog.h \
    mypixmapitem.h \
    imageprocessor.h \
    gaussblurdialog.h \
    biateraldialog.h \
    mediandialog.h \
    dilationdialog.h \
    erodedialog.h \
    matrix.h \
    fft.h \
    affinedialog.h \
    stretchdialog.h \
    logpara.h \
    lpdialog.h \
    hpdialog.h

FORMS += \
        mainwindow.ui \
    dialoglog.ui \
    gaussblurdialog.ui \
    biateraldialog.ui \
    mediandialog.ui \
    dilationdialog.ui \
    erodedialog.ui \
    affinedialog.ui \
    stretchdialog.ui \
    logpara.ui \
    lpdialog.ui \
    hpdialog.ui
