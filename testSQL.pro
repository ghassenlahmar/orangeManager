#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T13:04:28
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    addinterfacedialog.cpp \
    logindialog.cpp \
    vpntab.cpp \
    addvpndialog.cpp \
    ipcalculator.cpp \
    addsitedialog.cpp \
    adddetaildialog.cpp \
    siteviewer.cpp \
    interfaceviewer.cpp \
    confirmdeletedialog.cpp \
    newvpngroupdialog.cpp \
    databaseoperations.cpp

HEADERS  += \
    mainwindow.h \
    addinterfacedialog.h \
    logindialog.h \
    vpntab.h \
    addvpndialog.h \
    ipcalculator.h \
    addsitedialog.h \
    adddetaildialog.h \
    siteviewer.h \
    interfaceviewer.h \
    confirmdeletedialog.h \
    newvpngroupdialog.h \
    databaseoperations.h

FORMS    += \
    mainwindow.ui \
    addinterfacedialog.ui \
    logindialog.ui \
    vpntab.ui \
    addvpndialog.ui \
    addsitedialog.ui \
    adddetaildialog.ui \
    siteviewer.ui \
    interfaceviewer.ui \
    confirmdeletedialog.ui \
    newvpngroupdialog.ui
