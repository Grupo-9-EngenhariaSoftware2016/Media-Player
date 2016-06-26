#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T23:32:25
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = media4
TEMPLATE = app

win32: RC_ICONS = ucmusic.ico

SOURCES += main.cpp\
    dialog.cpp \
    mainwindow.cpp \
    classes.cpp \
    database.cpp \
    mydelegate.cpp \
    searchartist.cpp

HEADERS  += \
    dialog.h \
    mainwindow.h \
    classes.h \
    database.h \
    mydelegate.h \
    searchartist.h

FORMS    += mainwindow.ui \
    dialog.ui \
    searchartist.ui

RESOURCES += \
    icons/img.qrc
