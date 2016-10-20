CONFIG += c++11

include(capnproto.pri)

CAPNP_SCHEMAS += \
    $$PWD/position.capnp

LIBS += -lcapnp
