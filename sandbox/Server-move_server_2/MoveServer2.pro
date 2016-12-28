QT += core
QT += network
QT += gui

CONFIG += c++11

TARGET = MoveServer2
CONFIG += console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    server.cpp \
    machineinfo.cpp \
    game.cpp \
    mapinfo.cpp

HEADERS += \
    client.h \
    server.h \
    machineinfo.h \
    game.h \
    mapinfo.h
