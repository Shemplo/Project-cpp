QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = MoveServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    moveserver.cpp \
    serverconnection.cpp

HEADERS += \
    moveserver.h \
    serverconnection.h
