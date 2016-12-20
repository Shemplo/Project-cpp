#-------------------------------------------------
#
# Project created by QtCreator 2016-12-19T09:44:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrawClient2
TEMPLATE = app

win32-msvc* {
    LIBS += -luser32
}

SOURCES += main.cpp\
        drawclient.cpp \
    gamescene.cpp \
    dron.cpp \
    bullet.cpp

HEADERS  += drawclient.h \
    gamescene.h \
    dron.h \
    bullet.h

FORMS    += drawclient.ui
