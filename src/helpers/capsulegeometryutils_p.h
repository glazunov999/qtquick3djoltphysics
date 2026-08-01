#ifndef CAPSULEGEOMETRYUTILS_P_H
#define CAPSULEGEOMETRYUTILS_P_H

#include <QVector3D>

namespace CapsuleGeometryUtils {

inline QVector3D rotatedZ90(const QVector3D &vector)
{
    return QVector3D(vector.y(), -vector.x(), vector.z());
}

} // namespace CapsuleGeometryUtils

#endif // CAPSULEGEOMETRYUTILS_P_H
