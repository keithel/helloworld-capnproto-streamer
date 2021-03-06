#include "qposition.h"

void QPosition::setPosition(const Position &pos)
{
    setHeading(pos.heading);
    setElevation(pos.elevation);
    setLatitude(pos.latitude);
    setLongitude(pos.longitude);
    setHeightAboveEllipsoid(pos.heightAboveEllipsoid);
    setRoll(pos.roll);
}

void QPosition::setHeading(float heading)
{
    m_position.heading = heading;
    emit headingChanged();
}

void QPosition::setElevation(float elevation)
{
    m_position.elevation = elevation;
    emit elevationChanged();
}

void QPosition::setLatitude(float latitude)
{
    m_position.latitude = latitude;
    emit latitudeChanged();
}

void QPosition::setLongitude(float longitude)
{
    m_position.longitude = longitude;
    emit longitudeChanged();
}

void QPosition::setHeightAboveEllipsoid(float heightAboveEllipsoid)
{
    m_position.heightAboveEllipsoid = heightAboveEllipsoid;
    emit heightAboveEllipsoidChanged();
}

void QPosition::setRoll(float roll)
{
    m_position.roll = roll;
    emit rollChanged();
}


