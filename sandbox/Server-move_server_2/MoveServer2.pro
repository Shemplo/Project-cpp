QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = MoveServer2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    server.cpp \
    machineinfo.cpp

HEADERS += \
    client.h \
    server.h \
    machineinfo.h
