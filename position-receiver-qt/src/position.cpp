#include "position.h"
#include <sstream>
#include <QDebugStateSaver>
#include <QString>

bool Position::operator !=(const Position &pos) const
{
    if (heading != pos.heading) return true;
    if (elevation != pos.elevation) return true;
    if (latitude != pos.latitude) return true;
    if (longitude != pos.longitude) return true;
    if (heightAboveEllipsoid != pos.heightAboveEllipsoid) return true;
    if (roll != pos.roll) return true;
    return false;
}

std::string Position::toStdString() const
{
    std::ostringstream sstream;
    sstream << "Position" << '(' << heading << ", " << elevation << ", "
            << latitude << ", " << longitude << ", " << heightAboveEllipsoid << ", "
            << roll << ')';
    return sstream.str();
}

QDebug operator <<(QDebug qdebug, const Position& pos)
{
    QDebugStateSaver saver(qdebug);
    qdebug.nospace();
    qdebug << QString::fromStdString(pos.toStdString());
    return qdebug;
}
