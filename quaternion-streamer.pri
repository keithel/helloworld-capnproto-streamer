CONFIG += c++11

include(capnproto.pri)

CAPNP_SCHEMAS += \
    $$PWD/quaternion.capnp

LIBS += -lcapnp -lkj
