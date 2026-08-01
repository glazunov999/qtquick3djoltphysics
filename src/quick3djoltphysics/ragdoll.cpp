#include "ragdoll_p.h"
#include "physicsutils_p.h"
#include "skeletonpose_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/BodyLockInterface.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/Skeleton/Skeleton.h>

#include <utility>

template<typename Func>
void Ragdoll::applyPart(RagdollPart *part, Func &&func) const
{
    if (m_bodyInterface == nullptr)
        return;

    const JPH::BodyID bodyId = bodyIdForPart(part);
    if (!bodyId.IsInvalid())
        func(part, bodyId);
}

template<typename Func>
void Ragdoll::applyParts(Func &&func) const
{
    for (RagdollPart *part : std::as_const(m_parts))
        applyPart(part, func);
}

Ragdoll::Ragdoll(QQuick3DNode *parent)
    : AbstractPhysicsNode(parent)
{
}

Ragdoll::~Ragdoll()
{
    Ragdoll::cleanup();
}

const JPH::Skeleton *Ragdoll::skeleton() const
{
    const_cast<Ragdoll *>(this)->rebuildSkeletonFromParts();
    return m_skeleton;
}

QQmlListProperty<RagdollPart> Ragdoll::parts()
{
    return QQmlListProperty<RagdollPart>(this, this,
                                         &Ragdoll::appendPart,
                                         &Ragdoll::partCount,
                                         &Ragdoll::partAt,
                                         &Ragdoll::clearPartsList);
}

Body::MotionType Ragdoll::motionType() const
{
    return m_motionType;
}

void Ragdoll::setMotionType(Body::MotionType motionType)
{
    if (m_motionType == motionType)
        return;

    m_motionType = motionType;
    emit motionTypeChanged(m_motionType);

    applyParts([&](RagdollPart *part, JPH::BodyID bodyId) {
        m_bodyInterface->SetMotionType(bodyId, part->resolvedMotionType(m_motionType),
                                       JPH::EActivation::Activate);
    });

    refreshPartBodyMotionTypes();
}

void Ragdoll::setMotionTypeBelow(const QString &jointName, Body::MotionType type, bool includeSelf)
{
    if (m_joltRagdoll == nullptr || m_bodyInterface == nullptr || m_joltSettings == nullptr)
        return;

    const int rootIndex = jointIndexForName(jointName);
    if (rootIndex < 0)
        return;

    const JPH::Skeleton *skeleton = m_joltSettings->GetSkeleton();
    if (skeleton == nullptr)
        return;

    const int jointCount = skeleton->GetJointCount();
    QList<QList<int>> children;
    children.resize(jointCount);
    for (int i = 0; i < jointCount; ++i) {
        const int parentIndex = skeleton->GetJoint(i).mParentJointIndex;
        if (parentIndex >= 0)
            children[parentIndex].append(i);
    }

    QList<int> queue;
    if (includeSelf)
        queue.append(rootIndex);
    else
        queue.append(children.at(rootIndex));

    const auto joltType = static_cast<JPH::EMotionType>(type);
    while (!queue.isEmpty()) {
        const int jointIndex = queue.takeFirst();
        const JPH::BodyID bodyId = m_joltRagdoll->GetBodyID(jointIndex);
        if (!bodyId.IsInvalid())
            m_bodyInterface->SetMotionType(bodyId, joltType, JPH::EActivation::Activate);

        queue.append(children.at(jointIndex));
    }

    refreshPartBodyMotionTypes();
}

Body::MotionQuality Ragdoll::motionQuality() const
{
    return m_motionQuality;
}

void Ragdoll::setMotionQuality(Body::MotionQuality motionQuality)
{
    if (m_motionQuality == motionQuality)
        return;

    m_motionQuality = motionQuality;
    emit motionQualityChanged(m_motionQuality);

    applyParts([&](RagdollPart *part, JPH::BodyID bodyId) {
        m_bodyInterface->SetMotionQuality(bodyId, part->resolvedMotionQuality(m_motionQuality));
    });
}

int Ragdoll::objectLayer() const
{
    return m_objectLayer;
}

void Ragdoll::setObjectLayer(int objectLayer)
{
    if (m_objectLayer == objectLayer)
        return;

    m_objectLayer = objectLayer;
    emit objectLayerChanged(m_objectLayer);

    applyParts([&](RagdollPart *part, JPH::BodyID bodyId) {
        m_bodyInterface->SetObjectLayer(bodyId, part->resolvedObjectLayer(m_objectLayer));
    });
}

quint32 Ragdoll::collisionGroupId() const
{
    return m_collisionGroupId;
}

void Ragdoll::setCollisionGroupId(quint32 collisionGroupId)
{
    if (m_collisionGroupId == collisionGroupId)
        return;

    if (m_joltRagdoll != nullptr) {
        qWarning() << "Warning: Changing 'collisionGroupId' after ragdoll is initialized will have no effect";
        return;
    }

    m_collisionGroupId = collisionGroupId;
    emit collisionGroupIdChanged(m_collisionGroupId);
}

bool Ragdoll::simulationEnabled() const
{
    return m_simulationEnabled;
}

void Ragdoll::setSimulationEnabled(bool simulationEnabled)
{
    if (m_simulationEnabled == simulationEnabled)
        return;

    m_simulationEnabled = simulationEnabled;
    emit simulationEnabledChanged(m_simulationEnabled);

    if (m_joltRagdoll == nullptr)
        return;

    if (m_simulationEnabled)
        m_joltRagdoll->AddToPhysicsSystem(static_cast<JPH::EActivation>(m_activation));
    else
        m_joltRagdoll->RemoveFromPhysicsSystem();
}

void Ragdoll::activate()
{
    if (m_joltRagdoll != nullptr)
        m_joltRagdoll->Activate();
}

bool Ragdoll::isActive() const
{
    if (m_joltRagdoll == nullptr)
        return false;
    return m_joltRagdoll->IsActive();
}

void Ragdoll::ensurePoseSkeleton(SkeletonPose *pose) const
{
    if (pose == nullptr || m_joltRagdoll == nullptr)
        return;

    const JPH::Skeleton *skeleton = m_joltRagdoll->GetRagdollSettings()->GetSkeleton();
    if (pose->skeleton() != skeleton)
        pose->setSkeleton(skeleton);
}

void Ragdoll::getPose(SkeletonPose *pose) const
{
    if (m_joltRagdoll == nullptr || pose == nullptr)
        return;

    ensurePoseSkeleton(pose);
    pose->getFromRagdoll(m_joltRagdoll);
}

void Ragdoll::setPose(SkeletonPose *pose, bool lockBodies, bool resetWarmStart)
{
    if (m_joltRagdoll == nullptr || pose == nullptr)
        return;

    ensurePoseSkeleton(pose);
    pose->calculateJointMatrices();
    m_joltRagdoll->SetPose(pose->m_pose, lockBodies);

    if (resetWarmStart)
        m_joltRagdoll->ResetWarmStart();
}

void Ragdoll::driveToPoseUsingKinematics(SkeletonPose *pose, float deltaTime)
{
    if (m_joltRagdoll == nullptr || pose == nullptr)
        return;

    ensurePoseSkeleton(pose);
    pose->calculateJointMatrices();
    m_joltRagdoll->DriveToPoseUsingKinematics(pose->m_pose, deltaTime);
}

void Ragdoll::driveToPoseUsingMotors(SkeletonPose *pose)
{
    if (m_joltRagdoll == nullptr || pose == nullptr)
        return;

    ensurePoseSkeleton(pose);
    m_joltRagdoll->DriveToPoseUsingMotors(pose->m_pose);
}

void Ragdoll::driveToPoseUsingMotors(SkeletonPose *prevPose, SkeletonPose *pose, float deltaTime)
{
    if (m_joltRagdoll == nullptr || prevPose == nullptr || pose == nullptr)
        return;

    ensurePoseSkeleton(prevPose);
    ensurePoseSkeleton(pose);
    m_joltRagdoll->DriveToPoseUsingMotors(prevPose->m_pose, pose->m_pose, deltaTime);
}

void Ragdoll::setLinearAndAngularVelocity(const QVector3D &linearVelocity,
                                          const QVector3D &angularVelocity,
                                          bool lockBodies)
{
    if (m_joltRagdoll != nullptr) {
        m_joltRagdoll->SetLinearAndAngularVelocity(PhysicsUtils::toJoltType(linearVelocity),
                                                   PhysicsUtils::toJoltType(angularVelocity),
                                                   lockBodies);
    }
}

void Ragdoll::setLinearVelocity(const QVector3D &linearVelocity, bool lockBodies)
{
    if (m_joltRagdoll != nullptr)
        m_joltRagdoll->SetLinearVelocity(PhysicsUtils::toJoltType(linearVelocity), lockBodies);
}

void Ragdoll::addLinearVelocity(const QVector3D &linearVelocity, bool lockBodies)
{
    if (m_joltRagdoll != nullptr)
        m_joltRagdoll->AddLinearVelocity(PhysicsUtils::toJoltType(linearVelocity), lockBodies);
}

void Ragdoll::addImpulse(const QVector3D &impulse)
{
    if (m_joltRagdoll != nullptr)
        m_joltRagdoll->AddImpulse(PhysicsUtils::toJoltType(impulse));
}

void Ragdoll::addImpulse(const QString &jointName, const QVector3D &impulse, const QVector3D &point)
{
    if (m_joltRagdoll == nullptr || m_bodyInterface == nullptr)
        return;

    const int idx = jointIndexForName(jointName);
    if (idx < 0)
        return;

    const JPH::BodyID bodyId = m_joltRagdoll->GetBodyID(idx);
    if (bodyId.IsInvalid())
        return;

    m_bodyInterface->AddImpulse(bodyId,
                                PhysicsUtils::toJoltType(impulse),
                                PhysicsUtils::toJoltType(point));
}

int Ragdoll::bodyCount() const
{
    if (m_joltRagdoll == nullptr)
        return 0;
    return int(m_joltRagdoll->GetBodyCount());
}

Body *Ragdoll::body(const QString &jointName) const
{
    for (RagdollPart *part : std::as_const(m_parts)) {
        if (part->resolvedJointName() == jointName)
            return part->body();
    }

    return nullptr;
}

void Ragdoll::attachPartBodies()
{
    if (m_joltRagdoll == nullptr || m_jolt == nullptr || m_bodyInterface == nullptr)
        return;

    const JPH::BodyLockInterface &lockInterface = m_jolt->GetBodyLockInterface();

    for (RagdollPart *part : std::as_const(m_parts)) {
        const JPH::BodyID bodyId = bodyIdForPart(part);
        if (bodyId.IsInvalid())
            continue;

        JPH::Body *joltBody = lockInterface.TryGetBody(bodyId);
        if (joltBody == nullptr)
            continue;

        Body *body = part->body();
        if (body == nullptr) {
            body = new Body;
            body->setParent(part);
            connect(body, &QObject::destroyed, part, [part](QObject *obj) {
                if (part->body() == obj)
                    part->setBody(nullptr);
            });
            part->setBody(body);
        }

        body->attachJoltBody(m_jolt, m_bodyInterface, joltBody);
    }
}

void Ragdoll::detachPartBodies()
{
    for (RagdollPart *part : std::as_const(m_parts)) {
        if (Body *body = part->body()) {
            part->setBody(nullptr);
            delete body;
        }
    }
}

void Ragdoll::refreshPartBodyMotionTypes() const
{
    for (RagdollPart *part : std::as_const(m_parts)) {
        if (Body *body = part->body())
            body->refreshMotionTypeFromJolt();
    }
}

void Ragdoll::rebuildNameToIndex(const JPH::Skeleton *skeleton)
{
    m_nameToIndex.clear();
    if (skeleton == nullptr)
        return;

    const int jointCount = skeleton->GetJointCount();
    m_nameToIndex.reserve(jointCount);
    for (int i = 0; i < jointCount; ++i) {
        const JPH::Skeleton::Joint &joint = skeleton->GetJoint(i);
        m_nameToIndex[QString::fromUtf8(joint.mName.c_str())] = i;
    }
}

int Ragdoll::jointIndexForName(const QString &name) const
{
    const auto it = m_nameToIndex.constFind(name);
    if (it == m_nameToIndex.constEnd()) {
        qWarning() << "Ragdoll: unknown joint name" << name;
        return -1;
    }
    return it.value();
}

JPH::BodyID Ragdoll::bodyIdForPart(const RagdollPart *part) const
{
    if (m_joltRagdoll == nullptr || part == nullptr)
        return JPH::BodyID();

    const QString name = part->resolvedJointName();
    if (name.isEmpty())
        return JPH::BodyID();

    const int idx = jointIndexForName(name);
    if (idx < 0)
        return JPH::BodyID();

    return m_joltRagdoll->GetBodyID(idx);
}

void Ragdoll::rebuildSkeletonFromParts()
{
    if (!m_skeletonDirty)
        return;

    m_skeletonDirty = false;

    QHash<const RagdollPart *, int> partToIndex;
    partToIndex.reserve(m_parts.size());

    QList<RagdollPart *> resolvedParts;
    resolvedParts.reserve(m_parts.size());

    QList<QPair<QString, int>> joints;
    joints.reserve(m_parts.size());

    for (RagdollPart *part : std::as_const(m_parts)) {
        const QString name = part->resolvedJointName();
        if (name.isEmpty()) {
            qWarning() << "Ragdoll: part with empty jointName/objectName skipped";
            continue;
        }

        partToIndex.insert(part, joints.size());
        resolvedParts.append(part);
        joints.append(qMakePair(name, -1));
    }

    for (int i = 0; i < resolvedParts.size(); ++i) {
        int parentIndex = -1;
        for (QQuick3DNode *node = resolvedParts.at(i)->parentNode(); node != nullptr;
             node = node->parentNode()) {
            if (auto *parentPart = qobject_cast<RagdollPart *>(node)) {
                const auto it = partToIndex.constFind(parentPart);
                if (it != partToIndex.cend()) {
                    parentIndex = it.value();
                    break;
                }
            }
        }
        joints[i].second = parentIndex;
    }

    if (joints.isEmpty()) {
        m_skeleton = nullptr;
        return;
    }

    auto skeleton = new JPH::Skeleton;
    for (const auto &entry : joints) {
        const QByteArray name = entry.first.toUtf8();
        skeleton->AddJoint(name.constData(), entry.second);
    }
    skeleton->CalculateParentJointIndices();
    m_skeleton = skeleton;
}

JPH::Ref<JPH::RagdollSettings> Ragdoll::createJoltSettings() const
{
    const_cast<Ragdoll *>(this)->rebuildSkeletonFromParts();

    if (m_skeleton == nullptr)
        return nullptr;

    auto settings = new JPH::RagdollSettings;
    settings->mSkeleton = m_skeleton;
    settings->mParts.resize(m_skeleton->GetJointCount());

    for (const RagdollPart *part : std::as_const(m_parts)) {
        const QString name = part->resolvedJointName();
        if (name.isEmpty()) {
            qWarning() << "Ragdoll: part with empty jointName/objectName skipped";
            continue;
        }

        const QByteArray jointName = name.toUtf8();
        const int jointIndex = m_skeleton->GetJointIndex(jointName.constData());
        if (jointIndex < 0) {
            qWarning() << "Ragdoll: unknown joint" << name;
            continue;
        }

        settings->mParts[jointIndex] = part->createJoltPart(m_motionType, m_motionQuality, m_objectLayer);
    }

    for (int i = 0; i < m_skeleton->GetJointCount(); ++i) {
        if (settings->mParts[i].GetShape() == nullptr) {
            qWarning() << "Ragdoll: missing part for joint"
                       << m_skeleton->GetJoint(i).mName.c_str();
        }
    }

    if (!settings->Stabilize())
        qWarning() << "Ragdoll: Stabilize failed";

    settings->CalculateConstraintPriorities();
    settings->DisableParentChildCollisions();
    settings->CalculateBodyIndexToConstraintIndex();

    return settings;
}

void Ragdoll::handlePartChange()
{
    m_skeletonDirty = true;

    emit changed();

    if (m_jolt != nullptr)
        markJoltObjectDirty();
}

void Ragdoll::connectPart(RagdollPart *part)
{
    connect(part, &RagdollPart::shapeChanged, this, [this] { handlePartChange(); });
    connect(part, &RagdollPart::jointNameChanged, this, [this] { handlePartChange(); });
    connect(part, &RagdollPart::massChanged, this, [this] { handlePartChange(); });
    connect(part, &RagdollPart::constraintToParentChanged, this, [this] { handlePartChange(); });
    connect(part, &RagdollPart::motionTypeChanged, this, [this, part] {
        applyPart(part, [&](RagdollPart *p, JPH::BodyID bodyId) {
            m_bodyInterface->SetMotionType(bodyId, p->resolvedMotionType(m_motionType),
                                           JPH::EActivation::Activate);
            if (Body *body = p->body())
                body->refreshMotionTypeFromJolt();
        });
    });
    connect(part, &RagdollPart::motionQualityChanged, this, [this, part] {
        applyPart(part, [&](RagdollPart *p, JPH::BodyID bodyId) {
            m_bodyInterface->SetMotionQuality(bodyId, p->resolvedMotionQuality(m_motionQuality));
        });
    });
    connect(part, &RagdollPart::objectLayerChanged, this, [this, part] {
        applyPart(part, [&](RagdollPart *p, JPH::BodyID bodyId) {
            m_bodyInterface->SetObjectLayer(bodyId, p->resolvedObjectLayer(m_objectLayer));
        });
    });
    connect(part, &QObject::objectNameChanged, this, [this] { handlePartChange(); });
    connect(part, &QObject::destroyed, this, [this](QObject *obj) {
        m_parts.removeAll(static_cast<RagdollPart *>(obj));
        handlePartChange();
    });
}

void Ragdoll::disconnectPart(RagdollPart *part)
{
    part->disconnect(this);
}

void Ragdoll::appendPart(QQmlListProperty<RagdollPart> *list, RagdollPart *part)
{
    auto *ragdoll = qobject_cast<Ragdoll *>(list->object);
    if (!ragdoll || part == nullptr)
        return;

    if (ragdoll->m_parts.contains(part))
        return;

    ragdoll->connectPart(part);
    ragdoll->m_parts.append(part);
    ragdoll->handlePartChange();
}

qsizetype Ragdoll::partCount(QQmlListProperty<RagdollPart> *list)
{
    auto *ragdoll = qobject_cast<Ragdoll *>(list->object);
    return ragdoll ? ragdoll->m_parts.size() : 0;
}

RagdollPart *Ragdoll::partAt(QQmlListProperty<RagdollPart> *list, qsizetype index)
{
    auto *ragdoll = qobject_cast<Ragdoll *>(list->object);
    if (!ragdoll || index < 0 || index >= ragdoll->m_parts.size())
        return nullptr;
    return ragdoll->m_parts.at(index);
}

void Ragdoll::clearPartsList(QQmlListProperty<RagdollPart> *list)
{
    auto *ragdoll = qobject_cast<Ragdoll *>(list->object);
    if (!ragdoll)
        return;

    for (RagdollPart *part : std::as_const(ragdoll->m_parts))
        ragdoll->disconnectPart(part);

    ragdoll->m_parts.clear();
    ragdoll->handlePartChange();
}

void Ragdoll::updateJoltObject()
{
    if (m_jolt == nullptr || m_parts.isEmpty()) {
        cleanup();
        return;
    }

    cleanup();

    const JPH::Ref<JPH::RagdollSettings> settings = createJoltSettings();
    if (settings == nullptr)
        return;

    m_joltSettings = settings;
    rebuildNameToIndex(settings->GetSkeleton());
    // User data is assigned per part in attachPartBodies() so that ray casts and contact callbacks
    // resolve a limb to its own Body.
    m_joltRagdoll = settings->CreateRagdoll(static_cast<JPH::CollisionGroup::GroupID>(m_collisionGroupId),
                                            0,
                                            m_jolt);
    if (m_joltRagdoll == nullptr) {
        qWarning() << "Ragdoll: CreateRagdoll failed";
        m_joltSettings = nullptr;
        m_nameToIndex.clear();
        return;
    }

    if (m_simulationEnabled)
        m_joltRagdoll->AddToPhysicsSystem(static_cast<JPH::EActivation>(m_activation));

    attachPartBodies();
}

void Ragdoll::cleanup()
{
    detachPartBodies();

    if (m_joltRagdoll != nullptr) {
        m_joltRagdoll->RemoveFromPhysicsSystem();
        delete m_joltRagdoll;
        m_joltRagdoll = nullptr;
    }

    m_joltSettings = nullptr;
    m_nameToIndex.clear();
}

void Ragdoll::sync()
{
    if (m_joltRagdoll == nullptr || m_bodyInterface == nullptr)
        return;

    for (RagdollPart *part : std::as_const(m_parts)) {
        const QString name = part->resolvedJointName();
        if (name.isEmpty())
            continue;

        const int idx = jointIndexForName(name);
        if (idx < 0)
            continue;

        const JPH::BodyID bodyId = m_joltRagdoll->GetBodyID(idx);
        const QVector3D scenePosition = PhysicsUtils::toQtType(m_bodyInterface->GetPosition(bodyId));
        const QQuaternion sceneRotation = PhysicsUtils::toQtType(m_bodyInterface->GetRotation(bodyId));
        part->syncFromScene(scenePosition, sceneRotation);
    }
}
