#ifndef POSITION_H
#define POSITION_H

#include <QDebug>
#include <string>

class Position {

public:
    Position()
        : heading(0.0f), elevation(0.0f)
        , latitude(0.0f), longitude(0.0f)
        , heightAboveEllipsoid(0.0f)
        , roll(0.0f)
    {}

    Position(float nHeading, float nElevation, float nLatitude, float nLongitude, float nHeightAboveEllipsoid, float nRoll)
        : heading(nHeading), elevation(nElevation)
        , latitude(nLatitude), longitude(nLongitude)
        , heightAboveEllipsoid(nHeightAboveEllipsoid)
        , roll(nRoll)
    {}

    bool operator !=(const Position& pos) const;
    std::string toStdString() const;

public:
    float heading;
    float elevation;
    float latitude;
    float longitude;
    float heightAboveEllipsoid;
    float roll;
};

QDebug operator <<(QDebug qdebug, const Position& pos);

#endif // POSITION_H
