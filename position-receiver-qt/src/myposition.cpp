#include "myposition.h"
#include <sstream>
#include <QDebugStateSaver>
#include <QString>

void MyPosition::copyPosition(const MyPosition &pos)
{
    setHeading(pos.m_heading);
    setElevation(pos.m_elevation);
    setLatitude(pos.m_latitude);
    setLongitude(pos.m_longitude);
    setHeightAboveEllipsoid(pos.m_heightAboveEllipsoid);
    setRoll(pos.m_roll);
}

void MyPosition::setHeading(float heading)
{
    m_heading = heading;
    emit headingChanged();
}

void MyPosition::setElevation(float elevation)
{
    m_elevation = elevation;
    emit elevationChanged();
}

void MyPosition::setLatitude(float latitude)
{
    m_latitude = latitude;
    emit latitudeChanged();
}

void MyPosition::setLongitude(float longitude)
{
    m_longitude = longitude;
    emit longitudeChanged();
}

void MyPosition::setHeightAboveEllipsoid(float heightAboveEllipsoid)
{
    m_heightAboveEllipsoid = heightAboveEllipsoid;
    emit heightAboveEllipsoidChanged();
}

void MyPosition::setRoll(float roll)
{
    m_roll = roll;
    emit rollChanged();
}



bool MyPosition::operator !=(const MyPosition &pos) const
{
    if (m_heading != pos.m_heading) return true;
    if (m_elevation != pos.m_elevation) return true;
    if (m_latitude != pos.m_latitude) return true;
    if (m_longitude != pos.m_longitude) return true;
    if (m_heightAboveEllipsoid != pos.m_heightAboveEllipsoid) return true;
    if (m_roll != pos.m_roll) return true;
    return false;
}

std::string MyPosition::toStdString() const
{
    std::ostringstream sstream;
    sstream << "MyPosition" << '(' << m_heading << ", " << m_elevation << ", "
            << m_latitude << ", " << m_longitude << ", " << m_heightAboveEllipsoid << ", "
            << m_roll << ')';
    return sstream.str();
}

QDebug operator <<(QDebug qdebug, const MyPosition& pos)
{
    QDebugStateSaver saver(qdebug);
    qdebug.nospace();
    qdebug << QString::fromStdString(pos.toStdString());
    return qdebug;
}
