QT += qml quick network
CONFIG += c++11
TARGET = quaternion-receiver

include(../quaternion-streamer.pri)

HEADERS += \
    quaternionsocket.h

SOURCES += main.cpp \
    quaternionsocket.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

