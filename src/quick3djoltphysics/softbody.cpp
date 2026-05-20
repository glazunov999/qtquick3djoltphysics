#include "softbodymeshgeometry.h"
#include "softbody_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/SoftBody/SoftBodyMotionProperties.h>

SoftBody::SoftBody(QQuick3DNode *parent)
    : AbstractPhysicsNode(parent)
{
}

SoftBody::~SoftBody()
{
    SoftBody::cleanup();
}

SoftBodySharedSettings *SoftBody::sharedSettings() const
{
    return m_sharedSettings;
}

void SoftBody::setSharedSettings(SoftBodySharedSettings *settings)
{
    if (m_sharedSettings == settings)
        return;

    if (m_sharedSettings)
        m_sharedSettings->disconnect(this);

    m_sharedSettings = settings;

    if (m_sharedSettings) {
        connect(m_sharedSettings, &SoftBodySharedSettings::topologyChanged, this, [this]() {
            if (m_bodyInterface)
                updateJoltObject();
        });
    }

    emit sharedSettingsChanged(m_sharedSettings);

    if (m_bodyInterface)
        updateJoltObject();
}

quint32 SoftBody::objectLayer() const
{
    return m_objectLayer;
}

void SoftBody::setObjectLayer(quint32 objectLayer)
{
    if (m_objectLayer == objectLayer)
        return;
    m_objectLayer = objectLayer;
    emit objectLayerChanged(m_objectLayer);
}

int SoftBody::numIterations() const
{
    return m_numIterations;
}

void SoftBody::setNumIterations(int numIterations)
{
    if (m_numIterations == numIterations)
        return;
    m_numIterations = numIterations;
    emit numIterationsChanged(m_numIterations);
}

float SoftBody::linearDamping() const
{
    return m_linearDamping;
}

void SoftBody::setLinearDamping(float linearDamping)
{
    if (qFuzzyCompare(m_linearDamping, linearDamping))
        return;
    m_linearDamping = linearDamping;
    emit linearDampingChanged(m_linearDamping);
}

float SoftBody::maxLinearVelocity() const
{
    return m_maxLinearVelocity;
}

void SoftBody::setMaxLinearVelocity(float maxLinearVelocity)
{
    if (qFuzzyCompare(m_maxLinearVelocity, maxLinearVelocity))
        return;
    m_maxLinearVelocity = maxLinearVelocity;
    emit maxLinearVelocityChanged(m_maxLinearVelocity);
}

float SoftBody::restitution() const
{
    return m_restitution;
}

void SoftBody::setRestitution(float restitution)
{
    if (qFuzzyCompare(m_restitution, restitution))
        return;
    m_restitution = restitution;
    emit restitutionChanged(m_restitution);
}

float SoftBody::friction() const
{
    return m_friction;
}

void SoftBody::setFriction(float friction)
{
    if (qFuzzyCompare(m_friction, friction))
        return;
    m_friction = friction;
    emit frictionChanged(m_friction);
}

float SoftBody::pressure() const
{
    return m_pressure;
}

void SoftBody::setPressure(float pressure)
{
    if (qFuzzyCompare(m_pressure, pressure))
        return;
    m_pressure = pressure;
    emit pressureChanged(m_pressure);
}

float SoftBody::gravityFactor() const
{
    return m_gravityFactor;
}

void SoftBody::setGravityFactor(float gravityFactor)
{
    if (qFuzzyCompare(m_gravityFactor, gravityFactor))
        return;
    m_gravityFactor = gravityFactor;
    emit gravityFactorChanged(m_gravityFactor);
}

bool SoftBody::allowSleeping() const
{
    return m_allowSleeping;
}

void SoftBody::setAllowSleeping(bool allowSleeping)
{
    if (m_allowSleeping == allowSleeping)
        return;
    m_allowSleeping = allowSleeping;
    emit allowSleepingChanged(m_allowSleeping);
}

bool SoftBody::updatePosition() const
{
    return m_updatePosition;
}

void SoftBody::setUpdatePosition(bool updatePosition)
{
    if (m_updatePosition == updatePosition)
        return;
    m_updatePosition = updatePosition;
    emit updatePositionChanged(m_updatePosition);
}

bool SoftBody::makeRotationIdentity() const
{
    return m_makeRotationIdentity;
}

void SoftBody::setMakeRotationIdentity(bool makeRotationIdentity)
{
    if (m_makeRotationIdentity == makeRotationIdentity)
        return;
    m_makeRotationIdentity = makeRotationIdentity;
    emit makeRotationIdentityChanged(m_makeRotationIdentity);
}

quint32 SoftBody::bodyID() const
{
    return m_bodyID;
}

void SoftBody::setPressureRuntime(float pressure)
{
    if (!m_body || !m_body->IsSoftBody())
        return;
    auto *mp = static_cast<JPH::SoftBodyMotionProperties *>(m_body->GetMotionProperties());
    mp->SetPressure(pressure);
    m_pressure = pressure;
}

void SoftBody::setNumIterationsRuntime(int numIterations)
{
    if (!m_body || !m_body->IsSoftBody())
        return;
    auto *mp = static_cast<JPH::SoftBodyMotionProperties *>(m_body->GetMotionProperties());
    mp->SetNumIterations(static_cast<uint32_t>(numIterations));
    m_numIterations = numIterations;
}

QVector3D SoftBody::centerOfMassPosition() const
{
    if (!m_body)
        return {};
    return PhysicsUtils::toQtType(m_body->GetCenterOfMassPosition());
}

void SoftBody::setVertexVelocity(int vertexIndex, const QVector3D &velocity)
{
    if (!m_jolt || !m_body || !m_body->IsSoftBody())
        return;

    JPH::BodyLockWrite lock(m_jolt->GetBodyLockInterface(), m_body->GetID());
    if (!lock.Succeeded())
        return;

    auto *mp = static_cast<JPH::SoftBodyMotionProperties *>(lock.GetBody().GetMotionProperties());
    JPH::Array<JPH::SoftBodyVertex> &vertices = mp->GetVertices();
    if (vertexIndex < 0 || vertexIndex >= int(vertices.size()))
        return;

    vertices[static_cast<uint32_t>(vertexIndex)].mVelocity = PhysicsUtils::toJoltType(velocity);
}

void SoftBody::addForce(const QVector3D &force)
{
    if (!m_body || !m_bodyInterface)
        return;

    m_bodyInterface->AddForce(m_body->GetID(), PhysicsUtils::toJoltType(force));
}

void SoftBody::updateJoltObject()
{
    if (!m_sharedSettings) {
        qWarning() << "SoftBody: sharedSettings is required";
        return;
    }

    cleanup();

    JPH::Ref<JPH::SoftBodySharedSettings> joltSettings = m_sharedSettings->createJoltSettings();
    if (joltSettings == nullptr)
        return;

    if (joltSettings->mVertices.empty()) {
        qWarning() << "SoftBody: sharedSettings produced no vertices";
        return;
    }

    JPH::SoftBodyCreationSettings creationSettings(
        joltSettings,
        PhysicsUtils::toJoltType(scenePosition()),
        PhysicsUtils::toJoltType(sceneRotation()),
        static_cast<JPH::ObjectLayer>(m_objectLayer));

    creationSettings.mNumIterations = static_cast<uint32_t>(m_numIterations);
    creationSettings.mLinearDamping = m_linearDamping;
    creationSettings.mMaxLinearVelocity = m_maxLinearVelocity;
    creationSettings.mRestitution = m_restitution;
    creationSettings.mFriction = m_friction;
    creationSettings.mPressure = m_pressure;
    creationSettings.mGravityFactor = m_gravityFactor;
    creationSettings.mAllowSleeping = m_allowSleeping;
    creationSettings.mUpdatePosition = m_updatePosition;
    creationSettings.mMakeRotationIdentity = m_makeRotationIdentity;

    m_body = m_bodyInterface->CreateSoftBody(creationSettings);
    if (!m_body) {
        qWarning() << "SoftBody: failed to create Jolt soft body";
        return;
    }

    m_body->SetUserData(reinterpret_cast<JPH::uint64>(this));
    m_bodyInterface->AddBody(m_body->GetID(), JPH::EActivation::Activate);

    m_bodyID = m_body->GetID().GetIndexAndSequenceNumber();
    emit bodyIDChanged(m_bodyID);
    syncMeshGeometries();
}

void SoftBody::registerMeshGeometry(SoftBodyMeshGeometry *geometry)
{
    if (geometry)
        m_meshGeometries.insert(geometry);
}

void SoftBody::unregisterMeshGeometry(SoftBodyMeshGeometry *geometry)
{
    m_meshGeometries.remove(geometry);
}

void SoftBody::syncMeshGeometries()
{
    for (SoftBodyMeshGeometry *geometry : std::as_const(m_meshGeometries)) {
        geometry->invalidateRestUvs();
        geometry->updateFromSoftBody();
    }
}

void SoftBody::cleanup()
{
    if (m_body) {
        m_bodyInterface->RemoveBody(m_body->GetID());
        m_bodyInterface->DestroyBody(m_body->GetID());
        m_body = nullptr;
    }
}

void SoftBody::sync()
{
    if (!m_body)
        return;

    const auto pos = m_bodyInterface->GetPosition(m_body->GetID());
    const auto rotation = m_bodyInterface->GetRotation(m_body->GetID());

    setPosition(PhysicsUtils::toQtType(pos));
    setRotation(PhysicsUtils::toQtType(rotation));

    syncMeshGeometries();
}
