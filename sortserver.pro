QT       += core websockets
QT       -= gui

TARGET = sortserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    sortserver.cpp

HEADERS += \
    sortserver.h

