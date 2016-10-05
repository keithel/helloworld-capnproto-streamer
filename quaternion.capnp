@0xbfe8540109434e84;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("L3");

struct Quaternion
{
    scalar @0: Float32;
    xpos @1: Float32;
    ypos @2: Float32;
    zpos @3: Float32;
}
