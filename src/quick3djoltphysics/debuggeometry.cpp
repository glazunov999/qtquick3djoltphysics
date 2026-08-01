#include "debuggeometry_p.h"

#include <cstring>

DebugGeometry::DebugGeometry()
{
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Lines);
}

void DebugGeometry::updateVertices(const QVector<DebugLineVertex> &vertices)
{
    if (vertices.isEmpty()) {
        clearGeometry();
        return;
    }

    clear();
    setVertexData(QByteArray(reinterpret_cast<const char *>(vertices.constData()),
                             vertices.size() * int(sizeof(DebugLineVertex))));
    setStride(int(sizeof(DebugLineVertex)));
    setBounds(QVector3D(-1e6f, -1e6f, -1e6f), QVector3D(1e6f, 1e6f, 1e6f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Lines);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 offsetof(DebugLineVertex, position),
                 QQuick3DGeometry::Attribute::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::ColorSemantic,
                 offsetof(DebugLineVertex, color),
                 QQuick3DGeometry::Attribute::U32Type);
    QQuick3DGeometry::update();
}

void DebugGeometry::clearGeometry()
{
    clear();
    setVertexData({});
    setBounds({}, {});
    QQuick3DGeometry::update();
}
