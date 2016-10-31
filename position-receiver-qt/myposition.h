#ifndef MYPOSITION_H
#define MYPOSITION_H

#include <QDebug>
#include <string>

class MyPosition {
public:
    MyPosition(float heading, float elevation, float latitude, float longitude, float heightAboveEllipsoid, float roll)
        : m_heading(heading), m_elevation(elevation),
          m_latitude(latitude), m_longitude(longitude),
          m_heightAboveEllipsoid(heightAboveEllipsoid),
          m_roll(roll)
    {}
    bool operator !=(const MyPosition& pos);
    std::string toStdString();

public:
    float m_heading;
    float m_elevation;
    float m_latitude;
    float m_longitude;
    float m_heightAboveEllipsoid;
    float m_roll;
};

QDebug operator <<(QDebug qdebug, MyPosition pos);

#endif // MYPOSITION_H
