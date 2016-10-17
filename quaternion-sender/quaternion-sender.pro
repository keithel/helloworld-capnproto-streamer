QT += core
QT -= gui

CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

TARGET = quaternion-sender

include(../quaternion-streamer.pri)

HEADERS +=

SOURCES += main.cpp
