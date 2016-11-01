#ifndef QPOSITION_H
#define QPOSITION_H

#include <QDebug>
#include <string>
#include "position.h"

class QPosition : public QObject {
    Q_OBJECT
    Q_PROPERTY(float heading READ heading WRITE setHeading NOTIFY headingChanged);
    Q_PROPERTY(float elevation READ elevation WRITE setElevation NOTIFY elevationChanged);
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged);
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged);
    Q_PROPERTY(float heightAboveEllipsoid READ heightAboveEllipsoid WRITE setHeightAboveEllipsoid NOTIFY heightAboveEllipsoidChanged);
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged);

public:
    QPosition(QObject* parent = 0, const Position& pos = Position())
        : QObject(parent)
        , m_position(pos)
    {}

    void setPosition(const Position& pos);

    float heading() const { return m_position.heading; }
    void setHeading(float heading);
    float elevation() const { return m_position.elevation; }
    void setElevation(float elevation);
    float latitude() const { return m_position.latitude; }
    void setLatitude(float latitude);
    float longitude() const { return m_position.longitude; }
    void setLongitude(float longitude);
    float heightAboveEllipsoid() const { return m_position.heightAboveEllipsoid; }
    void setHeightAboveEllipsoid(float heightAboveEllipsoid);
    float roll() const { return m_position.roll; }
    void setRoll(float roll);

signals:
    void headingChanged();
    void elevationChanged();
    void latitudeChanged();
    void longitudeChanged();
    void heightAboveEllipsoidChanged();
    void rollChanged();


private:
    Position m_position;
};

#endif // QPOSITION_H
