#include "examplesoftbodycubemeshgeometry.h"
#include "examplesoftbodycubesettings.h"

#include <QtMath>

ExampleSoftBodyCubeMeshGeometry::ExampleSoftBodyCubeMeshGeometry() = default;

void ExampleSoftBodyCubeMeshGeometry::invalidateRestUvs()
{
    SoftBodyMeshGeometry::invalidateRestUvs();
    m_restPositionsValid = false;
    m_restPositions.clear();
}

void ExampleSoftBodyCubeMeshGeometry::ensureCachedData(const QVector<QVector3D> &restPositions,
                                                       SoftBodySharedSettings *settings)
{
    auto *cubeSettings = qobject_cast<ExampleSoftBodyCubeSettings *>(settings);
    if (!cubeSettings || cubeSettings->topologyVertexCount() != restPositions.size())
        return;

    if (m_restPositionsValid && m_restPositions.size() == restPositions.size())
        return;

    m_restPositions = restPositions;
    cubeSettings->cacheRestPositions(m_restPositions);
    m_restPositionsValid = true;
}

void ExampleSoftBodyCubeMeshGeometry::textureCoordinatesForTriangle(int i0,
                                                                    int i1,
                                                                    int i2,
                                                                    SoftBodySharedSettings *settings,
                                                                    QVector2D &uv0,
                                                                    QVector2D &uv1,
                                                                    QVector2D &uv2) const
{
    auto *cubeSettings = qobject_cast<ExampleSoftBodyCubeSettings *>(settings);
    if (!cubeSettings || !m_restPositionsValid || m_restPositions.size() <= qMax(i0, qMax(i1, i2))) {
        uv0 = {};
        uv1 = {};
        uv2 = {};
        return;
    }

    const QVector3D &rp0 = m_restPositions[i0];
    const QVector3D &rp1 = m_restPositions[i1];
    const QVector3D &rp2 = m_restPositions[i2];

    QVector3D restNormal = QVector3D::crossProduct(rp1 - rp0, rp2 - rp0);
    if (!restNormal.isNull())
        restNormal.normalize();

    uv0 = cubeSettings->textureCoordinateForFace(i0, rp0, restNormal);
    uv1 = cubeSettings->textureCoordinateForFace(i1, rp1, restNormal);
    uv2 = cubeSettings->textureCoordinateForFace(i2, rp2, restNormal);
}
