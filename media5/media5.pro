#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T23:32:25
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = media5
TEMPLATE = app


SOURCES += main.cpp\
    dialog.cpp \
    mainwindow.cpp \
    classes.cpp \
    database.cpp

HEADERS  += \
    dialog.h \
    mainwindow.h \
    classes.h \
    database.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    icons/img.qrc
