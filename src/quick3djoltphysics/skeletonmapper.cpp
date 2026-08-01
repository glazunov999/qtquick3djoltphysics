#include "skeletonmapper_p.h"
#include "ragdoll_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>

namespace {

JPH::SkeletonPose::Mat44Vector toAbsoluteWorldSpace(const JPH::SkeletonPose &pose)
{
    const JPH::SkeletonPose::Mat44Vector &matrices = pose.GetJointMatrices();
    const JPH::RVec3 rootOffset = pose.GetRootOffset();

    JPH::SkeletonPose::Mat44Vector absolute;
    absolute.resize(matrices.size());
    for (size_t i = 0; i < matrices.size(); ++i) {
        absolute[i] = matrices[i];
        absolute[i].SetTranslation(matrices[i].GetTranslation() + rootOffset);
    }
    return absolute;
}

JPH::SkeletonPose::Mat44Vector ragdollPoseInSkinModelSpace(const JPH::SkeletonPose &pose,
                                                           QQuick3DNode *modelNode)
{
    return Quick3DSkeletonUtils::worldToModelSpace(modelNode, toAbsoluteWorldSpace(pose));
}

} // namespace

SkeletonMapper::SkeletonMapper(QObject *parent)
    : QObject(parent)
{
}

SkeletonMapper::~SkeletonMapper() = default;

Ragdoll *SkeletonMapper::ragdoll() const
{
    return m_ragdoll;
}

void SkeletonMapper::setRagdoll(Ragdoll *ragdoll)
{
    if (m_ragdoll == ragdoll)
        return;

    if (m_ragdoll != nullptr)
        m_ragdoll->disconnect(this);

    m_ragdoll = ragdoll;

    if (m_ragdoll != nullptr) {
        connect(m_ragdoll, &Ragdoll::changed, this, [this] { invalidate(); });
        connect(m_ragdoll, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_ragdoll == obj)
                setRagdoll(nullptr);
        });
    }

    invalidate();
    emit ragdollChanged(m_ragdoll);
}

QQuick3DSkin *SkeletonMapper::skin() const
{
    return m_skin;
}

void SkeletonMapper::setSkin(QQuick3DSkin *skin)
{
    if (m_skin == skin)
        return;

    if (m_skin != nullptr)
        m_skin->disconnect(this);

    m_skin = skin;

    if (m_skin != nullptr) {
        connect(m_skin, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_skin == obj)
                setSkin(nullptr);
        });
    }

    invalidate();
    emit skinChanged(m_skin);
}

QQuick3DNode *SkeletonMapper::modelNode() const
{
    return m_modelNode;
}

void SkeletonMapper::setModelNode(QQuick3DNode *modelNode)
{
    if (m_modelNode == modelNode)
        return;

    if (m_modelNode != nullptr)
        m_modelNode->disconnect(this);

    m_modelNode = modelNode;

    if (m_modelNode != nullptr) {
        connect(m_modelNode, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_modelNode == obj)
                setModelNode(nullptr);
        });
    }

    invalidate();
    emit modelNodeChanged(m_modelNode);
}

SkeletonPose *SkeletonMapper::neutralPose() const
{
    return m_neutralPose;
}

void SkeletonMapper::setNeutralPose(SkeletonPose *neutralPose)
{
    if (m_neutralPose == neutralPose)
        return;

    if (m_neutralPose != nullptr)
        m_neutralPose->disconnect(this);

    m_neutralPose = neutralPose;

    if (m_neutralPose != nullptr) {
        connect(m_neutralPose, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_neutralPose == obj)
                setNeutralPose(nullptr);
        });
    }

    invalidate();
    emit neutralPoseChanged(m_neutralPose);
}

bool SkeletonMapper::lockTranslations() const
{
    return m_lockTranslations;
}

void SkeletonMapper::setLockTranslations(bool lockTranslations)
{
    if (m_lockTranslations == lockTranslations)
        return;

    m_lockTranslations = lockTranslations;
    invalidate();
    emit lockTranslationsChanged(m_lockTranslations);
}

bool SkeletonMapper::isInitialized() const
{
    return m_initialized;
}

bool SkeletonMapper::initialize()
{
    invalidate();

    if (m_ragdoll == nullptr || m_skin == nullptr || m_modelNode == nullptr
        || m_neutralPose == nullptr) {
        qWarning() << "SkeletonMapper: ragdoll, skin, modelNode and neutralPose are required";
        return false;
    }

    const JPH::Skeleton *skeleton = m_ragdoll->skeleton();
    if (skeleton == nullptr) {
        qWarning() << "SkeletonMapper: ragdoll has no skeleton (parts missing?)";
        return false;
    }

    m_ragdollJoltSkeleton = skeleton;
    m_renderJoints = Quick3DSkeletonUtils::collectRenderJoints(m_skin);
    m_renderJoltSkeleton = Quick3DSkeletonUtils::createJoltSkeleton(m_renderJoints);

    if (m_ragdollJoltSkeleton == nullptr || m_renderJoltSkeleton == nullptr) {
        qWarning() << "SkeletonMapper: failed to create Jolt skeletons";
        m_renderJoints.clear();
        return false;
    }

    const int ragdollJointCount = int(m_ragdollJoltSkeleton->GetJointCount());
    const int renderJointCount = int(m_renderJoltSkeleton->GetJointCount());
    if (ragdollJointCount > renderJointCount) {
        qWarning() << "SkeletonMapper: ragdoll skeleton has more joints than render skeleton"
                   << "(" << ragdollJointCount << ">" << renderJointCount << ")";
        return false;
    }

    if (m_renderJoints.isEmpty()) {
        qWarning() << "SkeletonMapper: Skin has no joints";
        return false;
    }

    if (m_neutralPose->jointCount() != ragdollJointCount) {
        qWarning() << "SkeletonMapper: ragdoll neutral pose joint count does not match ragdoll skeleton";
        return false;
    }

    if (m_neutralPose->m_pose.GetJointMatrices().size() != size_t(ragdollJointCount)) {
        qWarning() << "SkeletonMapper: ragdoll neutral pose has no joint matrices (call getPose first)";
        return false;
    }

    m_renderNeutralModelMatrices = Quick3DSkeletonUtils::getModelSpacePose(m_modelNode, m_renderJoints);
    m_renderNeutralLocalMatrices = Quick3DSkeletonUtils::getLocalSpacePose(m_renderJoints);

    if (m_renderNeutralModelMatrices.size() != renderJointCount) {
        qWarning() << "SkeletonMapper: render neutral pose joint count mismatch";
        return false;
    }

    const JPH::SkeletonPose::Mat44Vector ragdollNeutralModelPose =
        ragdollPoseInSkinModelSpace(m_neutralPose->m_pose, m_modelNode);

    m_mapper = new JPH::SkeletonMapper;
    m_mapper->Initialize(m_ragdollJoltSkeleton,
                         ragdollNeutralModelPose.data(),
                         m_renderJoltSkeleton,
                         m_renderNeutralModelMatrices.data());

    for (int i = 0; i < ragdollJointCount; ++i) {
        if (m_mapper->GetMappedJointIdx(i) >= 0)
            continue;

        qWarning() << "SkeletonMapper: ragdoll joint"
                   << m_ragdollJoltSkeleton->GetJoint(i).mName.c_str()
                   << "has no matching render joint";
        invalidate();
        return false;
    }

    if (m_lockTranslations)
        m_mapper->LockAllTranslations(m_renderJoltSkeleton, m_renderNeutralModelMatrices.data());

    m_renderModelScratch.resize(m_renderNeutralModelMatrices.size());
    m_initialized = true;
    emit initializedChanged(m_initialized);
    return true;
}

void SkeletonMapper::mapReverse(SkeletonPose *ragdollPose)
{
    if (!m_initialized || m_mapper == nullptr || m_skin == nullptr || m_modelNode == nullptr
        || ragdollPose == nullptr) {
        qWarning() << "SkeletonMapper::mapReverse: mapper is not initialized or ragdollPose is null";
        return;
    }

    if (ragdollPose->jointCount() != int(m_ragdollJoltSkeleton->GetJointCount())) {
        qWarning() << "SkeletonMapper::mapReverse: ragdoll pose joint count mismatch";
        return;
    }

    const JPH::SkeletonPose::Mat44Vector renderModelPose =
        Quick3DSkeletonUtils::getModelSpacePose(m_modelNode, m_renderJoints);

    JPH::SkeletonPose::Mat44Vector ragdollModelPose;
    ragdollModelPose.resize(ragdollPose->m_pose.GetJointMatrices().size());
    m_mapper->MapReverse(renderModelPose.data(), ragdollModelPose.data());

    const JPH::Mat44 modelToWorld = PhysicsUtils::toJoltType(m_modelNode->sceneTransform());
    for (size_t i = 0; i < ragdollModelPose.size(); ++i)
        ragdollModelPose[i] = modelToWorld * ragdollModelPose[i];

    if (!ragdollModelPose.empty()) {
        const JPH::Vec3 rootTranslation = ragdollModelPose[0].GetTranslation();
        ragdollPose->m_pose.SetRootOffset(JPH::RVec3(rootTranslation));
        ragdollModelPose[0].SetTranslation(JPH::Vec3::sZero());
        for (size_t i = 1; i < ragdollModelPose.size(); ++i)
            ragdollModelPose[i].SetTranslation(ragdollModelPose[i].GetTranslation() - rootTranslation);
        ragdollPose->m_pose.GetJointMatrices() = ragdollModelPose;
    }

    ragdollPose->calculateJointStates();
}

void SkeletonMapper::map(SkeletonPose *ragdollPose)
{
    if (!m_initialized || m_mapper == nullptr || m_skin == nullptr || m_modelNode == nullptr
        || ragdollPose == nullptr) {
        qWarning() << "SkeletonMapper::map: mapper is not initialized or ragdollPose is null";
        return;
    }

    if (ragdollPose->jointCount() != int(m_ragdollJoltSkeleton->GetJointCount())) {
        qWarning() << "SkeletonMapper::map: ragdoll pose joint count mismatch";
        return;
    }

    if (ragdollPose->m_pose.GetJointMatrices().size() != size_t(m_ragdollJoltSkeleton->GetJointCount())) {
        qWarning() << "SkeletonMapper::map: ragdoll pose has no joint matrices (call getPose first)";
        return;
    }

    const JPH::SkeletonPose::Mat44Vector ragdollModelPose =
        ragdollPoseInSkinModelSpace(ragdollPose->m_pose, m_modelNode);

    m_mapper->Map(ragdollModelPose.data(),
                  m_renderNeutralLocalMatrices.data(),
                  m_renderModelScratch.data());
    Quick3DSkeletonUtils::applyModelSpacePose(m_renderJoints, m_renderModelScratch);
}

int SkeletonMapper::mappedJointIndex(int ragdollJointIndex) const
{
    if (!m_initialized || m_mapper == nullptr)
        return -1;

    return m_mapper->GetMappedJointIdx(ragdollJointIndex);
}

bool SkeletonMapper::isJointTranslationLocked(int renderJointIndex) const
{
    if (!m_initialized || m_mapper == nullptr)
        return false;

    return m_mapper->IsJointTranslationLocked(renderJointIndex);
}

void SkeletonMapper::invalidate()
{
    const bool wasInitialized = m_initialized;
    m_initialized = false;
    m_mapper = nullptr;
    m_ragdollJoltSkeleton = nullptr;
    m_renderJoltSkeleton = nullptr;
    m_renderJoints.clear();
    m_renderNeutralModelMatrices.clear();
    m_renderNeutralLocalMatrices.clear();
    m_renderModelScratch.clear();

    if (wasInitialized)
        emit initializedChanged(m_initialized);
}
