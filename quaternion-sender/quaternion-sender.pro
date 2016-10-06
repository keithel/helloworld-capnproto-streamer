QT += core network

CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

TARGET = quaternion-sender

include(../quaternion-streamer.pri)

HEADERS += \
    quaternionsocket.h

SOURCES += main.cpp \
    quaternionsocket.cpp

LIBS += -lkj
