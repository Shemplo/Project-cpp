QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = MoveClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    moveclient.cpp

HEADERS += \
    moveclient.h
