#ifndef SKELETONMAPPER_P_H
#define SKELETONMAPPER_P_H

#include "quick3dskeletonutils_p.h"
#include "skeletonpose_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQuick3D/private/qquick3dskin_p.h>
#include <QtQml/QQmlEngine>
#include <QObject>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Skeleton/SkeletonMapper.h>

class Ragdoll;

class Q_QUICK3DJOLTPHYSICS_EXPORT SkeletonMapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Ragdoll *ragdoll READ ragdoll WRITE setRagdoll NOTIFY ragdollChanged)
    Q_PROPERTY(QQuick3DSkin *skin READ skin WRITE setSkin NOTIFY skinChanged)
    Q_PROPERTY(QQuick3DNode *modelNode READ modelNode WRITE setModelNode NOTIFY modelNodeChanged)
    Q_PROPERTY(SkeletonPose *neutralPose READ neutralPose WRITE setNeutralPose NOTIFY neutralPoseChanged)
    Q_PROPERTY(bool lockTranslations READ lockTranslations WRITE setLockTranslations NOTIFY lockTranslationsChanged)
    Q_PROPERTY(bool initialized READ isInitialized NOTIFY initializedChanged)
    QML_NAMED_ELEMENT(SkeletonMapper)
public:
    explicit SkeletonMapper(QObject *parent = nullptr);
    ~SkeletonMapper() override;

    Ragdoll *ragdoll() const;
    void setRagdoll(Ragdoll *ragdoll);

    QQuick3DSkin *skin() const;
    void setSkin(QQuick3DSkin *skin);

    QQuick3DNode *modelNode() const;
    void setModelNode(QQuick3DNode *modelNode);

    SkeletonPose *neutralPose() const;
    void setNeutralPose(SkeletonPose *neutralPose);

    bool lockTranslations() const;
    void setLockTranslations(bool lockTranslations);

    bool isInitialized() const;

    Q_INVOKABLE bool initialize();
    Q_INVOKABLE void mapReverse(SkeletonPose *ragdollPose);
    Q_INVOKABLE void map(SkeletonPose *ragdollPose);

    Q_INVOKABLE int mappedJointIndex(int ragdollJointIndex) const;
    Q_INVOKABLE bool isJointTranslationLocked(int renderJointIndex) const;

signals:
    void ragdollChanged(Ragdoll *ragdoll);
    void skinChanged(QQuick3DSkin *skin);
    void modelNodeChanged(QQuick3DNode *modelNode);
    void neutralPoseChanged(SkeletonPose *neutralPose);
    void lockTranslationsChanged(bool lockTranslations);
    void initializedChanged(bool initialized);

private:
    void invalidate();

    Ragdoll *m_ragdoll = nullptr;
    QQuick3DSkin *m_skin = nullptr;
    QQuick3DNode *m_modelNode = nullptr;
    SkeletonPose *m_neutralPose = nullptr;
    bool m_lockTranslations = false;
    bool m_initialized = false;

    QVector<Quick3DSkeletonUtils::RenderJoint> m_renderJoints;
    JPH::SkeletonPose::Mat44Vector m_renderNeutralModelMatrices;
    JPH::SkeletonPose::Mat44Vector m_renderNeutralLocalMatrices;
    JPH::SkeletonPose::Mat44Vector m_renderModelScratch;

    JPH::RefConst<JPH::Skeleton> m_ragdollJoltSkeleton;
    JPH::Ref<JPH::Skeleton> m_renderJoltSkeleton;
    JPH::Ref<JPH::SkeletonMapper> m_mapper;
};

#endif // SKELETONMAPPER_P_H
