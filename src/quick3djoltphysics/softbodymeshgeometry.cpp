#include "softbodymeshgeometry.h"
#include "softbody_p.h"
#include "softbodysharedsettings.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/SoftBody/SoftBodyMotionProperties.h>

#include <QVector3D>
#include <QtMath>

#include <cstddef>
#include <cstring>

namespace {

struct VertexPNT
{
    QVector3D position;
    QVector3D normal;
    QVector2D uv;
};

} // namespace

SoftBodyMeshGeometry::SoftBodyMeshGeometry()
{
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
}

SoftBody *SoftBodyMeshGeometry::softBody() const
{
    return m_softBody;
}

void SoftBodyMeshGeometry::invalidateRestUvs()
{
}

void SoftBodyMeshGeometry::removeTopologyConnection()
{
    if (!m_topologySettings)
        return;

    disconnect(m_topologySettings, &SoftBodySharedSettings::topologyChanged, this,
               &SoftBodyMeshGeometry::invalidateRestUvs);
    m_topologySettings = nullptr;
}

void SoftBodyMeshGeometry::updateTopologyConnection()
{
    removeTopologyConnection();

    if (!m_softBody || !m_softBody->sharedSettings())
        return;

    m_topologySettings = m_softBody->sharedSettings();
    connect(m_topologySettings, &SoftBodySharedSettings::topologyChanged, this,
            &SoftBodyMeshGeometry::invalidateRestUvs);
}

void SoftBodyMeshGeometry::setSoftBody(SoftBody *softBody)
{
    if (m_softBody == softBody)
        return;

    if (m_softBody) {
        m_softBody->unregisterMeshGeometry(this);
        disconnect(m_softBody, nullptr, this, nullptr);
    }

    removeTopologyConnection();
    invalidateRestUvs();
    m_softBody = softBody;

    if (m_softBody) {
        m_softBody->registerMeshGeometry(this);
        connect(m_softBody, &SoftBody::sharedSettingsChanged, this,
                &SoftBodyMeshGeometry::updateTopologyConnection);
        updateTopologyConnection();
    }

    emit softBodyChanged();
    updateFromSoftBody();
}

void SoftBodyMeshGeometry::ensureCachedData(const QVector<QVector3D> &restPositions,
                                            SoftBodySharedSettings *settings)
{
    Q_UNUSED(restPositions)
    Q_UNUSED(settings)
}

void SoftBodyMeshGeometry::textureCoordinatesForTriangle(int i0,
                                                         int i1,
                                                         int i2,
                                                         SoftBodySharedSettings *settings,
                                                         QVector2D &uv0,
                                                         QVector2D &uv1,
                                                         QVector2D &uv2) const
{
    Q_UNUSED(i0)
    Q_UNUSED(i1)
    Q_UNUSED(i2)
    Q_UNUSED(settings)

    uv0 = {};
    uv1 = {};
    uv2 = {};
}

void SoftBodyMeshGeometry::updateFromSoftBody()
{
    if (!m_softBody)
        return;

    JPH::Body *body = m_softBody->joltBody();
    if (!body || !body->IsSoftBody())
        return;

    auto *mp = static_cast<JPH::SoftBodyMotionProperties *>(body->GetMotionProperties());

    const JPH::Array<JPH::SoftBodyVertex> &vertices = mp->GetVertices();
    const JPH::Array<JPH::SoftBodySharedSettings::Face> &faces = mp->GetFaces();

    if (vertices.empty() || faces.empty())
        return;

    SoftBodySharedSettings *settings = m_softBody->sharedSettings();

    QVector<QVector3D> restPositions;
    restPositions.resize(int(vertices.size()));
    for (int i = 0; i < restPositions.size(); ++i)
        restPositions[i] = PhysicsUtils::toQtType(vertices[uint(i)].mPosition);

    ensureCachedData(restPositions, settings);

    QVector3D boundsMin;
    QVector3D boundsMax;
    bool boundsInit = false;

    QVector<VertexPNT> triangleVertices;
    triangleVertices.reserve(static_cast<int>(faces.size()) * 3);

    for (const JPH::SoftBodySharedSettings::Face &face : faces) {
        const JPH::uint32 i0 = face.mVertex[0];
        const JPH::uint32 i1 = face.mVertex[1];
        const JPH::uint32 i2 = face.mVertex[2];

        const QVector3D q0 = PhysicsUtils::toQtType(vertices[i0].mPosition);
        const QVector3D q1 = PhysicsUtils::toQtType(vertices[i1].mPosition);
        const QVector3D q2 = PhysicsUtils::toQtType(vertices[i2].mPosition);

        QVector3D normal = QVector3D::crossProduct(q1 - q0, q2 - q0);
        if (!normal.isNull())
            normal.normalize();

        QVector2D uv0;
        QVector2D uv1;
        QVector2D uv2;
        textureCoordinatesForTriangle(int(i0), int(i1), int(i2), settings, uv0, uv1, uv2);

        triangleVertices.append({ q0, normal, uv0 });
        triangleVertices.append({ q1, normal, uv1 });
        triangleVertices.append({ q2, normal, uv2 });

        for (const QVector3D &p : { q0, q1, q2 }) {
            if (!boundsInit) {
                boundsMin = p;
                boundsMax = p;
                boundsInit = true;
            } else {
                boundsMin.setX(qMin(boundsMin.x(), p.x()));
                boundsMin.setY(qMin(boundsMin.y(), p.y()));
                boundsMin.setZ(qMin(boundsMin.z(), p.z()));
                boundsMax.setX(qMax(boundsMax.x(), p.x()));
                boundsMax.setY(qMax(boundsMax.y(), p.y()));
                boundsMax.setZ(qMax(boundsMax.z(), p.z()));
            }
        }
    }

    constexpr int kStride = sizeof(VertexPNT);
    QByteArray vertexData(triangleVertices.size() * kStride, Qt::Initialization::Uninitialized);
    memcpy(vertexData.data(), triangleVertices.constData(), size_t(vertexData.size()));

    clear();
    setVertexData(vertexData);
    setStride(kStride);
    setBounds(boundsMin, boundsMax);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::NormalSemantic,
                 offsetof(VertexPNT, normal),
                 QQuick3DGeometry::Attribute::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::TexCoord0Semantic,
                 offsetof(VertexPNT, uv),
                 QQuick3DGeometry::Attribute::F32Type);
    update();
}
