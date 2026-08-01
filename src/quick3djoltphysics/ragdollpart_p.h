#ifndef RAGDOLLPART_P_H
#define RAGDOLLPART_P_H

#include "abstractshape_p.h"
#include "abstracttwobodyphysicsconstraint_p.h"
#include "body_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>
#include <QQuaternion>
#include <QVector3D>

#include <Jolt/Physics/Ragdoll/Ragdoll.h>

class Ragdoll;

class Q_QUICK3DJOLTPHYSICS_EXPORT RagdollPart : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(QString jointName READ jointName WRITE setJointName NOTIFY jointNameChanged)
    Q_PROPERTY(AbstractShape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(MotionType motionType READ motionType WRITE setMotionType NOTIFY motionTypeChanged)
    Q_PROPERTY(MotionQuality motionQuality READ motionQuality WRITE setMotionQuality NOTIFY motionQualityChanged)
    Q_PROPERTY(int objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(AbstractTwoBodyPhysicsConstraintSettings *constraintToParent READ constraintToParent WRITE setConstraintToParent NOTIFY constraintToParentChanged)
    Q_PROPERTY(Body *body READ body NOTIFY bodyChanged)
    QML_NAMED_ELEMENT(RagdollPart)
public:
    enum class MotionType {
        Inherit,
        Kinematic,
        Dynamic,
    };
    Q_ENUM(MotionType)

    enum class MotionQuality {
        Inherit,
        Discrete,
        LinearCast,
    };
    Q_ENUM(MotionQuality)

    explicit RagdollPart(QQuick3DNode *parent = nullptr);
    ~RagdollPart() override;

    QString jointName() const;
    void setJointName(const QString &jointName);

    QString resolvedJointName() const;

    AbstractShape *shape() const;
    void setShape(AbstractShape *shape);

    float mass() const;
    void setMass(float mass);

    MotionType motionType() const;
    void setMotionType(MotionType motionType);

    MotionQuality motionQuality() const;
    void setMotionQuality(MotionQuality motionQuality);

    int objectLayer() const;
    void setObjectLayer(int objectLayer);

    AbstractTwoBodyPhysicsConstraintSettings *constraintToParent() const;
    void setConstraintToParent(AbstractTwoBodyPhysicsConstraintSettings *constraintToParent);

    Body *body() const;

signals:
    void jointNameChanged(const QString &jointName);
    void shapeChanged(AbstractShape *shape);
    void massChanged(float mass);
    void motionTypeChanged(MotionType motionType);
    void motionQualityChanged(MotionQuality motionQuality);
    void objectLayerChanged(int objectLayer);
    void constraintToParentChanged(AbstractTwoBodyPhysicsConstraintSettings *constraintToParent);
    void bodyChanged(Body *body);

private:
    JPH::EMotionType resolvedMotionType(Body::MotionType ragdollDefault) const;
    JPH::EMotionQuality resolvedMotionQuality(Body::MotionQuality ragdollDefault) const;
    JPH::ObjectLayer resolvedObjectLayer(int ragdollDefault) const;

    JPH::RagdollSettings::Part createJoltPart(Body::MotionType defaultMotionType,
                                              Body::MotionQuality defaultMotionQuality,
                                              int defaultObjectLayer) const;
    void syncFromScene(const QVector3D &scenePosition, const QQuaternion &sceneRotation);
    void setBody(Body *body);

    QString m_jointName;
    AbstractShape *m_shape = nullptr;
    float m_mass = 0.0f;
    MotionType m_motionType = MotionType::Inherit;
    MotionQuality m_motionQuality = MotionQuality::Inherit;
    int m_objectLayer = -1;
    AbstractTwoBodyPhysicsConstraintSettings *m_constraintToParent = nullptr;
    Body *m_body = nullptr;
    QMetaObject::Connection m_shapeSignalConnection;
    QMetaObject::Connection m_constraintSignalConnection;

    friend class Ragdoll;
};

#endif // RAGDOLLPART_P_H
