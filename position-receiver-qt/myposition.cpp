#include "myposition.h"
#include <sstream>
#include <QDebugStateSaver>
#include <QString>

bool MyPosition::operator !=(const MyPosition &pos)
{
    if (m_heading != pos.m_heading) return true;
    if (m_elevation != pos.m_elevation) return true;
    if (m_latitude != pos.m_latitude) return true;
    if (m_longitude != pos.m_longitude) return true;
    if (m_heightAboveEllipsoid != pos.m_heightAboveEllipsoid) return true;
    return false;
}

std::string MyPosition::toStdString()
{
    std::ostringstream sstream;
    sstream << "MyPosition" << '(' << m_heading << ", " << m_elevation << ", " << m_latitude << ", " << m_longitude << ", " << m_heightAboveEllipsoid << ')';
    return sstream.str();
}

QDebug operator <<(QDebug qdebug, MyPosition pos)
{
    QDebugStateSaver saver(qdebug);
    qdebug.nospace();
    qdebug << QString::fromStdString(pos.toStdString());
    return qdebug;
}
