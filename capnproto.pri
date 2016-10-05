
!CONFIG(c++11): error("Capnproto extensively uses C++11 features, please enable c++11 in your CONFIG")

# To use this, please install Cap'n Proto version 0.5.3 or higher.
# On Ubuntu 14.04 Trusty Tahr:
#     sudo add-apt-repository ppa:me-jasonmartens/capnproto  # 0.5.3 backported to Trusty Tahr
#     sudo apt-get update
#     sudo apt-get install capnproto libcapnp-dev

QMAKE_EXT_CPP += .c++
INCLUDEPATH += $${OUT_PWD}/.capnp
capnp.commands = capnp compile ${QMAKE_FILE_NAME} --src-prefix=${QMAKE_FILE_PATH} -oc++:$${OUT_PWD}/.capnp
capnp.dependency_type = TYPE_C
capnp.input = CAPNP_SCHEMAS
capnp.output = $${OUT_PWD}/.capnp/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}.h
capnp.variable_out = SOURCES
capnp.clean = $${OUT_PWD}/.capnp/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}.h $${OUT_PWD}/.capnp/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}.c++
QMAKE_EXTRA_COMPILERS += capnp

