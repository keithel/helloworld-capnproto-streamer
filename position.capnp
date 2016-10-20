@0xbfe8540109434e84;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("L3");

struct Position
{
    heading              @0: Float32; # Heading in radians (-pi to pi).
    elevation            @1: Float32; # Elevation in radians (-pi/2 to pi/2).
    latitude             @2: Float32; # Latitude in WGS-84 degrees.
    longitude            @3: Float32; # Longitude in WGS-84 degrees.
    heightAboveEllipsoid @4: Float32; # Height Above WGS-84 Ellipsoid in meters.
}
