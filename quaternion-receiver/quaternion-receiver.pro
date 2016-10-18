QT += network concurrent
!nogui:qtHaveModule(gui) {
    QT += qml quick
}
else {
    QT += core
    QT -= gui
}

CONFIG += c++11
TARGET = quaternion-receiver

include(../quaternion-streamer.pri)

HEADERS += \
    quaternionsocket.h

SOURCES += main.cpp \
    quaternionsocket.cpp

!nogui:qtHaveModule(gui) {
    RESOURCES += qml.qrc

    # Additional import path used to resolve QML modules in Qt Creator's code model
    QML_IMPORT_PATH =
}

# Default rules for deployment.
include(deployment.pri)

