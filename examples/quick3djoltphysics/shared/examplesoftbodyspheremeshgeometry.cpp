#include "examplesoftbodyspheremeshgeometry.h"
#include "examplesoftbodyspheresettings.h"

ExampleSoftBodySphereMeshGeometry::ExampleSoftBodySphereMeshGeometry() = default;

void ExampleSoftBodySphereMeshGeometry::invalidateRestUvs()
{
    SoftBodyMeshGeometry::invalidateRestUvs();
    m_restUvsValid = false;
    m_restUvs.clear();
}

void ExampleSoftBodySphereMeshGeometry::ensureCachedData(const QVector<QVector3D> &restPositions,
                                                         SoftBodySharedSettings *settings)
{
    auto *sphereSettings = qobject_cast<ExampleSoftBodySphereSettings *>(settings);
    if (!sphereSettings || sphereSettings->topologyVertexCount() != restPositions.size())
        return;

    if (!sphereSettings->hasExplicitTextureCoordinates())
        return;

    const int vertexCount = restPositions.size();
    if (m_restUvsValid && m_restUvs.size() == vertexCount)
        return;

    m_restUvs.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i)
        m_restUvs[i] = sphereSettings->textureCoordinateAt(i);
    m_restUvsValid = true;
}

void ExampleSoftBodySphereMeshGeometry::textureCoordinatesForTriangle(int i0,
                                                                      int i1,
                                                                      int i2,
                                                                      SoftBodySharedSettings *settings,
                                                                      QVector2D &uv0,
                                                                      QVector2D &uv1,
                                                                      QVector2D &uv2) const
{
    Q_UNUSED(settings)

    uv0 = m_restUvsValid ? m_restUvs[i0] : QVector2D();
    uv1 = m_restUvsValid ? m_restUvs[i1] : QVector2D();
    uv2 = m_restUvsValid ? m_restUvs[i2] : QVector2D();
}
