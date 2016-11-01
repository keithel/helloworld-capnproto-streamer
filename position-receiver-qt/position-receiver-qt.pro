QT += network concurrent
!nogui:qtHaveModule(gui) {
    QT += qml quick
}
else {
    QT += core
    QT -= gui
}

CONFIG += c++11
TARGET = position-receiver-qt
#DEFINES += SHOW_HASPENDINGDATAGRAMS_BUG

include(../position-streamer.pri)

HEADERS += \
    src/positionreceiver.h \
    src/position.h \
    src/qposition.h \
    src/options.h

SOURCES += src/main.cpp \
    src/positionreceiver.cpp \
    src/position.cpp \
    src/qposition.cpp \
    src/options.cpp

!nogui:qtHaveModule(gui) {
    RESOURCES += qml/qml.qrc

    # Additional import path used to resolve QML modules in Qt Creator's code model
    QML_IMPORT_PATH =
}

# Default rules for deployment.
include(deployment.pri)

