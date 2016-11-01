#ifndef MYPOSITION_H
#define MYPOSITION_H

#include <QDebug>
#include <string>

class MyPosition : public QObject {
    Q_OBJECT
    Q_PROPERTY(float heading READ heading WRITE setHeading NOTIFY headingChanged);
    Q_PROPERTY(float elevation READ elevation WRITE setElevation NOTIFY elevationChanged);
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged);
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged);
    Q_PROPERTY(float heightAboveEllipsoid READ heightAboveEllipsoid WRITE setHeightAboveEllipsoid NOTIFY heightAboveEllipsoidChanged);
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged);

public:
    MyPosition(float heading, float elevation, float latitude, float longitude, float heightAboveEllipsoid, float roll)
        : m_heading(heading), m_elevation(elevation),
          m_latitude(latitude), m_longitude(longitude),
          m_heightAboveEllipsoid(heightAboveEllipsoid),
          m_roll(roll)
    {}

    void copyPosition(const MyPosition& pos);

    float heading() const { return m_heading; }
    void setHeading(float heading);
    float elevation() const { return m_elevation; }
    void setElevation(float elevation);
    float latitude() const { return m_latitude; }
    void setLatitude(float latitude);
    float longitude() const { return m_longitude; }
    void setLongitude(float longitude);
    float heightAboveEllipsoid() const { return m_heightAboveEllipsoid; }
    void setHeightAboveEllipsoid(float heightAboveEllipsoid);
    float roll() const { return m_roll; }
    void setRoll(float roll);

    bool operator !=(const MyPosition& pos) const;
    std::string toStdString() const;

signals:
    void headingChanged();
    void elevationChanged();
    void latitudeChanged();
    void longitudeChanged();
    void heightAboveEllipsoidChanged();
    void rollChanged();


private:
    float m_heading;
    float m_elevation;
    float m_latitude;
    float m_longitude;
    float m_heightAboveEllipsoid;
    float m_roll;
};

QDebug operator <<(QDebug qdebug, const MyPosition& pos);

#endif // MYPOSITION_H
