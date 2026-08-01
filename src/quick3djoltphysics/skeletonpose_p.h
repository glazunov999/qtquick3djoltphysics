#ifndef SKELETONPOSE_P_H
#define SKELETONPOSE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Skeleton/Skeleton.h>
#include <Jolt/Skeleton/SkeletonPose.h>

namespace JPH {
class Ragdoll;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT SkeletonPose : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D rootOffset READ rootOffset WRITE setRootOffset NOTIFY rootOffsetChanged)
    QML_NAMED_ELEMENT(SkeletonPose)
public:
    explicit SkeletonPose(QObject *parent = nullptr);
    ~SkeletonPose() override;

    const JPH::Skeleton *skeleton() const;
    void setSkeleton(const JPH::Skeleton *skeleton);

    QVector3D rootOffset() const;
    void setRootOffset(const QVector3D &rootOffset);

    Q_INVOKABLE int jointCount() const;
    Q_INVOKABLE int jointIndex(const QString &name) const;

    Q_INVOKABLE void setJointRotation(int jointIndex, const QQuaternion &rotation);
    Q_INVOKABLE void setJointTranslation(int jointIndex, const QVector3D &translation);
    Q_INVOKABLE QQuaternion jointRotation(int jointIndex) const;
    Q_INVOKABLE QVector3D jointTranslation(int jointIndex) const;

    Q_INVOKABLE void setJointRotationByName(const QString &name, const QQuaternion &rotation);
    Q_INVOKABLE void setJointTranslationByName(const QString &name, const QVector3D &translation);

    Q_INVOKABLE void calculateJointMatrices();
    Q_INVOKABLE void calculateJointStates();
    Q_INVOKABLE void calculateLocalSpaceJointMatrices();

    void getFromRagdoll(JPH::Ragdoll *ragdoll);

signals:
    void rootOffsetChanged(const QVector3D &rootOffset);

private:
    JPH::RefConst<JPH::Skeleton> m_skeleton;
    JPH::SkeletonPose m_pose;
    JPH::SkeletonPose::Mat44Vector m_localSpaceMatrices;

    friend class Ragdoll;
    friend class SkeletonMapper;
};

#endif // SKELETONPOSE_P_H
