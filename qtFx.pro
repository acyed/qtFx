#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T13:36:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtFx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    alienfx.h

FORMS    += mainwindow.ui


LIBS += -L/usr/local -lusb-1.0
INCLUDEPATH += /usr/local
