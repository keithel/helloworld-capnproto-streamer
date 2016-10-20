QT += network concurrent
!nogui:qtHaveModule(gui) {
    QT += qml quick
}
else {
    QT += core
    QT -= gui
}

CONFIG += c++11
TARGET = position-receiver

include(../position-streamer.pri)

HEADERS += \
    positionreceiver.h \
    myposition.h

SOURCES += main.cpp \
    positionreceiver.cpp \
    myposition.cpp

!nogui:qtHaveModule(gui) {
    RESOURCES += qml.qrc

    # Additional import path used to resolve QML modules in Qt Creator's code model
    QML_IMPORT_PATH =
}

# Default rules for deployment.
include(deployment.pri)
