#-------------------------------------------------
#
# Project created by QtCreator 2016-12-21T23:00:46
#
#-------------------------------------------------

QT += core 
QT += gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MoveClient3
TEMPLATE = app


SOURCES += main.cpp\
    userform.cpp \
    application.cpp \
    queueform.cpp

HEADERS  += \
    userform.h \
    application.h \
    queueform.h

FORMS    += \
    userform.ui \
    application.ui \
    queueform.ui

RESOURCES += \
    icons.qrc