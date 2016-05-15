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
<<<<<<< HEAD
    database.cpp\
    classes.cpp
=======
    classes.cpp \
    database.cpp
>>>>>>> origin/Bruno

HEADERS  += \
    dialog.h \
    mainwindow.h \
<<<<<<< HEAD
    database.h\
    classes.h
=======
    classes.h \
    database.h
>>>>>>> origin/Bruno

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    icons/img.qrc
