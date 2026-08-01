#include "skeletonpose_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>

SkeletonPose::SkeletonPose(QObject *parent)
    : QObject(parent)
{
}

SkeletonPose::~SkeletonPose() = default;

const JPH::Skeleton *SkeletonPose::skeleton() const
{
    return m_skeleton;
}

void SkeletonPose::setSkeleton(const JPH::Skeleton *skeleton)
{
    if (m_skeleton == skeleton)
        return;

    m_skeleton = skeleton;
    if (m_skeleton != nullptr)
        m_pose.SetSkeleton(m_skeleton);
    else
        m_localSpaceMatrices.clear();
}

QVector3D SkeletonPose::rootOffset() const
{
    return PhysicsUtils::toQtType(m_pose.GetRootOffset());
}

void SkeletonPose::setRootOffset(const QVector3D &rootOffset)
{
    const JPH::RVec3 joltOffset(rootOffset.x(), rootOffset.y(), rootOffset.z());
    if (m_pose.GetRootOffset() == joltOffset)
        return;

    m_pose.SetRootOffset(joltOffset);
    emit rootOffsetChanged(rootOffset);
}

int SkeletonPose::jointCount() const
{
    return static_cast<int>(m_pose.GetJointCount());
}

int SkeletonPose::jointIndex(const QString &name) const
{
    if (m_skeleton == nullptr)
        return -1;

    const int index = m_skeleton->GetJointIndex(name.toUtf8().constData());
    return index >= 0 ? index : -1;
}

void SkeletonPose::setJointRotation(int jointIndex, const QQuaternion &rotation)
{
    if (jointIndex < 0 || jointIndex >= static_cast<int>(m_pose.GetJointCount()))
        return;

    m_pose.GetJoint(jointIndex).mRotation = PhysicsUtils::toJoltType(rotation);
}

void SkeletonPose::setJointTranslation(int jointIndex, const QVector3D &translation)
{
    if (jointIndex < 0 || jointIndex >= static_cast<int>(m_pose.GetJointCount()))
        return;

    m_pose.GetJoint(jointIndex).mTranslation = PhysicsUtils::toJoltType(translation);
}

QQuaternion SkeletonPose::jointRotation(int jointIndex) const
{
    if (jointIndex < 0 || jointIndex >= static_cast<int>(m_pose.GetJointCount()))
        return {};

    return PhysicsUtils::toQtType(m_pose.GetJoint(jointIndex).mRotation);
}

QVector3D SkeletonPose::jointTranslation(int jointIndex) const
{
    if (jointIndex < 0 || jointIndex >= static_cast<int>(m_pose.GetJointCount()))
        return {};

    return PhysicsUtils::toQtType(m_pose.GetJoint(jointIndex).mTranslation);
}

void SkeletonPose::setJointRotationByName(const QString &name, const QQuaternion &rotation)
{
    setJointRotation(jointIndex(name), rotation);
}

void SkeletonPose::setJointTranslationByName(const QString &name, const QVector3D &translation)
{
    setJointTranslation(jointIndex(name), translation);
}

void SkeletonPose::calculateJointMatrices()
{
    m_pose.CalculateJointMatrices();
}

void SkeletonPose::calculateJointStates()
{
    m_pose.CalculateJointStates();
}

void SkeletonPose::calculateLocalSpaceJointMatrices()
{
    const int count = static_cast<int>(m_pose.GetJointCount());
    m_localSpaceMatrices.resize(count);
    if (count > 0)
        m_pose.CalculateLocalSpaceJointMatrices(m_localSpaceMatrices.data());
}

void SkeletonPose::getFromRagdoll(JPH::Ragdoll *ragdoll)
{
    if (ragdoll == nullptr)
        return;

    ragdoll->GetPose(m_pose);
    m_pose.CalculateJointStates();
    emit rootOffsetChanged(rootOffset());
}
