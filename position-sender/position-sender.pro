QT += core network
QT -= gui

CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

TARGET = position-sender

include(../position-streamer.pri)

HEADERS += \
    positionsender.h

SOURCES += main.cpp \
    positionsender.cpp
