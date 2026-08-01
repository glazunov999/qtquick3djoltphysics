#include "taperedcylindergeometry_p.h"

#include <QVector2D>
#include <QVector3D>
#include <QtMath>

QT_BEGIN_NAMESPACE

TaperedCylinderGeometry::TaperedCylinderGeometry()
{
    updateData();
}

void TaperedCylinderGeometry::setEnableNormals(bool enable)
{
    if (m_enableNormals == enable)
        return;

    m_enableNormals = enable;
    emit enableNormalsChanged();
    updateData();
    update();
}

void TaperedCylinderGeometry::setEnableUV(bool enable)
{
    if (m_enableUV == enable)
        return;

    m_enableUV = enable;
    emit enableUVChanged();
    updateData();
    update();
}

void TaperedCylinderGeometry::setLongitudes(int longitudes)
{
    if (m_longitudes == longitudes)
        return;

    m_longitudes = longitudes;
    emit longitudesChanged();
    updateData();
    update();
}

void TaperedCylinderGeometry::setHeight(float height)
{
    if (qFuzzyCompare(m_height, height))
        return;

    m_height = height;
    emit heightChanged();
    updateData();
    update();
}

void TaperedCylinderGeometry::setTopDiameter(float topDiameter)
{
    if (qFuzzyCompare(m_topDiameter, topDiameter))
        return;

    m_topDiameter = topDiameter;
    emit topDiameterChanged();
    updateData();
    update();
}

void TaperedCylinderGeometry::setBottomDiameter(float bottomDiameter)
{
    if (qFuzzyCompare(m_bottomDiameter, bottomDiameter))
        return;

    m_bottomDiameter = bottomDiameter;
    emit bottomDiameterChanged();
    updateData();
    update();
}

static QVector3D sideNormal(float s, float c, const QVector3D &top, const QVector3D &bottom)
{
    const QVector3D radial(s, 0.0f, c);
    const QVector3D edge = top - bottom;
    return QVector3D::crossProduct(edge, QVector3D::crossProduct(radial, edge)).normalized();
}

void TaperedCylinderGeometry::updateData()
{
    clear();

    const float halfHeight = m_height * 0.5f;
    const float topRadius = m_topDiameter * 0.5f;
    const float bottomRadius = m_bottomDiameter * 0.5f;
    const int segments = qMax(3, m_longitudes);

    QVector<QVector3D> positions;
    QVector<QVector3D> normals;
    QVector<QVector2D> uvs;
    QVector<quint32> indices;

    const auto addVertex = [&](const QVector3D &position, const QVector3D &normal, const QVector2D &uv) {
        positions.append(position);
        normals.append(normal);
        uvs.append(uv);
        return quint32(positions.size() - 1);
    };

    if (topRadius > 0.0f) {
        const quint32 topCenter = addVertex(QVector3D(0.0f, halfHeight, 0.0f),
                                            QVector3D(0.0f, 1.0f, 0.0f),
                                            QVector2D(0.5f, 0.5f));
        const quint32 topRingStart = positions.size();
        for (int i = 0; i <= segments; ++i) {
            const float angle = 2.0f * float(M_PI) * float(i) / segments;
            const float s = std::sin(angle);
            const float c = std::cos(angle);
            addVertex(QVector3D(topRadius * s, halfHeight, topRadius * c),
                      QVector3D(0.0f, 1.0f, 0.0f),
                      QVector2D(float(i) / segments, 1.0f));
        }
        for (int i = 0; i < segments; ++i) {
            indices << topCenter << topRingStart + i << topRingStart + i + 1;
        }
    }

    if (bottomRadius > 0.0f) {
        const quint32 bottomCenter = addVertex(QVector3D(0.0f, -halfHeight, 0.0f),
                                               QVector3D(0.0f, -1.0f, 0.0f),
                                               QVector2D(0.5f, 0.5f));
        const quint32 bottomRingStart = positions.size();
        for (int i = 0; i <= segments; ++i) {
            const float angle = 2.0f * float(M_PI) * float(i) / segments;
            const float s = std::sin(angle);
            const float c = std::cos(angle);
            addVertex(QVector3D(bottomRadius * s, -halfHeight, bottomRadius * c),
                      QVector3D(0.0f, -1.0f, 0.0f),
                      QVector2D(float(i) / segments, 0.0f));
        }
        for (int i = 0; i < segments; ++i) {
            indices << bottomCenter << bottomRingStart + i + 1 << bottomRingStart + i;
        }
    }

    const quint32 sideStart = positions.size();
    for (int i = 0; i <= segments; ++i) {
        const float angle = 2.0f * float(M_PI) * float(i) / segments;
        const float s = std::sin(angle);
        const float c = std::cos(angle);
        const QVector3D top(topRadius * s, halfHeight, topRadius * c);
        const QVector3D bottom(bottomRadius * s, -halfHeight, bottomRadius * c);
        const QVector3D normal = sideNormal(s, c, top, bottom);
        const float u = float(i) / segments;
        addVertex(top, normal, QVector2D(u, 1.0f));
        addVertex(bottom, normal, QVector2D(u, 0.0f));
    }

    for (int i = 0; i < segments; ++i) {
        const quint32 t0 = sideStart + 2 * i;
        const quint32 t1 = sideStart + 2 * (i + 1);
        const quint32 b0 = t0 + 1;
        const quint32 b1 = t1 + 1;
        indices << t0 << b0 << b1;
        indices << t0 << b1 << t1;
    }

    const int stride = 3 * sizeof(float) + (m_enableNormals ? 3 * sizeof(float) : 0)
            + (m_enableUV ? 2 * sizeof(float) : 0);
    const int strideNormal = 3 * sizeof(float);
    const int strideUV = strideNormal + (m_enableNormals ? 3 * sizeof(float) : 0);

    QByteArray vertexData(stride * positions.size(), Qt::Initialization::Uninitialized);
    QByteArray indexData(sizeof(quint32) * indices.size(), Qt::Initialization::Uninitialized);

    const auto getVertexPtr = [&](int vertexIdx) {
        return reinterpret_cast<QVector3D *>(vertexData.data() + stride * vertexIdx);
    };
    const auto getNormalPtr = [&](int vertexIdx) {
        return reinterpret_cast<QVector3D *>(vertexData.data() + stride * vertexIdx + strideNormal);
    };
    const auto getTexturePtr = [&](int vertexIdx) {
        return reinterpret_cast<QVector2D *>(vertexData.data() + stride * vertexIdx + strideUV);
    };

    quint32 *indexPtr = reinterpret_cast<quint32 *>(indexData.data());
    for (quint32 index : indices)
        *indexPtr++ = index;

    for (qsizetype i = 0; i < positions.size(); ++i) {
        *getVertexPtr(int(i)) = positions[i];
        if (m_enableNormals)
            *getNormalPtr(int(i)) = normals[i];
        if (m_enableUV)
            *getTexturePtr(int(i)) = uvs[i];
    }

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::F32Type);
    if (m_enableNormals) {
        addAttribute(QQuick3DGeometry::Attribute::NormalSemantic, strideNormal,
                     QQuick3DGeometry::Attribute::ComponentType::F32Type);
    }
    if (m_enableUV) {
        addAttribute(QQuick3DGeometry::Attribute::TexCoordSemantic, strideUV,
                     QQuick3DGeometry::Attribute::ComponentType::F32Type);
    }
    addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0,
                 QQuick3DGeometry::Attribute::ComponentType::U32Type);

    setStride(stride);
    setVertexData(vertexData);
    setIndexData(indexData);

    const float maxRadius = qMax(topRadius, bottomRadius);
    setBounds(QVector3D(-maxRadius, -halfHeight, -maxRadius),
              QVector3D(maxRadius, halfHeight, maxRadius));
}

QT_END_NAMESPACE
