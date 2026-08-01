#ifndef RAGDOLL_P_H
#define RAGDOLL_P_H

#include "abstractphysicsnode_p.h"
#include "body_p.h"
#include "ragdollpart_p.h"
#include "skeletonpose_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>
#include <QHash>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Skeleton/Skeleton.h>

namespace JPH {
class Ragdoll;
class RagdollSettings;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT Ragdoll : public AbstractPhysicsNode
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<RagdollPart> parts READ parts CONSTANT)
    Q_PROPERTY(Body::MotionType motionType READ motionType WRITE setMotionType NOTIFY motionTypeChanged)
    Q_PROPERTY(Body::MotionQuality motionQuality READ motionQuality WRITE setMotionQuality NOTIFY motionQualityChanged)
    Q_PROPERTY(int objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(quint32 collisionGroupId READ collisionGroupId WRITE setCollisionGroupId NOTIFY collisionGroupIdChanged)
    Q_PROPERTY(bool simulationEnabled READ simulationEnabled WRITE setSimulationEnabled NOTIFY simulationEnabledChanged)
    QML_NAMED_ELEMENT(Ragdoll)
public:
    explicit Ragdoll(QQuick3DNode *parent = nullptr);
    ~Ragdoll() override;

    const JPH::Skeleton *skeleton() const;

    QQmlListProperty<RagdollPart> parts();
    const QList<RagdollPart *> &partsList() const { return m_parts; }

    Body::MotionType motionType() const;
    void setMotionType(Body::MotionType motionType);

    Body::MotionQuality motionQuality() const;
    void setMotionQuality(Body::MotionQuality motionQuality);

    int objectLayer() const;
    void setObjectLayer(int objectLayer);

    quint32 collisionGroupId() const;
    void setCollisionGroupId(quint32 collisionGroupId);

    bool simulationEnabled() const;
    void setSimulationEnabled(bool simulationEnabled);

    Q_INVOKABLE void setMotionTypeBelow(const QString &jointName,
                                        Body::MotionType type,
                                        bool includeSelf = true);

    Q_INVOKABLE void activate();
    Q_INVOKABLE bool isActive() const;
    Q_INVOKABLE void getPose(SkeletonPose *pose) const;
    Q_INVOKABLE void setPose(SkeletonPose *pose, bool lockBodies = true, bool resetWarmStart = false);
    Q_INVOKABLE void driveToPoseUsingKinematics(SkeletonPose *pose, float deltaTime);
    Q_INVOKABLE void driveToPoseUsingMotors(SkeletonPose *pose);
    Q_INVOKABLE void driveToPoseUsingMotors(SkeletonPose *prevPose, SkeletonPose *pose, float deltaTime);
    Q_INVOKABLE void setLinearAndAngularVelocity(const QVector3D &linearVelocity,
                                                   const QVector3D &angularVelocity,
                                                   bool lockBodies = true);
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity, bool lockBodies = true);
    Q_INVOKABLE void addLinearVelocity(const QVector3D &linearVelocity, bool lockBodies = true);
    Q_INVOKABLE void addImpulse(const QVector3D &impulse);
    Q_INVOKABLE void addImpulse(const QString &jointName, const QVector3D &impulse, const QVector3D &point);
    Q_INVOKABLE int bodyCount() const;
    Q_INVOKABLE Body *body(const QString &jointName) const;

signals:
    void changed();
    void motionTypeChanged(Body::MotionType motionType);
    void motionQualityChanged(Body::MotionQuality motionQuality);
    void objectLayerChanged(int objectLayer);
    void collisionGroupIdChanged(quint32 collisionGroupId);
    void simulationEnabledChanged(bool simulationEnabled);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;

private:
    JPH::Ref<JPH::RagdollSettings> createJoltSettings() const;
    void handlePartChange();
    void connectPart(RagdollPart *part);
    void disconnectPart(RagdollPart *part);

    static void appendPart(QQmlListProperty<RagdollPart> *list, RagdollPart *part);
    static qsizetype partCount(QQmlListProperty<RagdollPart> *list);
    static RagdollPart *partAt(QQmlListProperty<RagdollPart> *list, qsizetype index);
    static void clearPartsList(QQmlListProperty<RagdollPart> *list);

    void rebuildNameToIndex(const JPH::Skeleton *skeleton);
    int jointIndexForName(const QString &name) const;
    void rebuildSkeletonFromParts();
    void ensurePoseSkeleton(SkeletonPose *pose) const;
    void attachPartBodies();
    void detachPartBodies();
    void refreshPartBodyMotionTypes() const;

    JPH::BodyID bodyIdForPart(const RagdollPart *part) const;

    template<typename Func>
    void applyPart(RagdollPart *part, Func &&func) const;
    template<typename Func>
    void applyParts(Func &&func) const;

    JPH::Ref<JPH::Skeleton> m_skeleton;
    QList<RagdollPart *> m_parts;
    Body::MotionType m_motionType = Body::MotionType::Dynamic;
    Body::MotionQuality m_motionQuality = Body::MotionQuality::Discrete;
    int m_objectLayer = 0;
    quint32 m_collisionGroupId = 0;
    bool m_simulationEnabled = true;
    bool m_skeletonDirty = true;

    JPH::RefConst<JPH::RagdollSettings> m_joltSettings;
    JPH::Ragdoll *m_joltRagdoll = nullptr;
    QHash<QString, int> m_nameToIndex;
};

#endif // RAGDOLL_P_H
