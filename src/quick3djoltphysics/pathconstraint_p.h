#ifndef PATHCONSTRAINT_P_H
#define PATHCONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"
#include "pathconstraintpath_p.h"
#include "motorsettings_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/PathConstraint.h>

#include <QVector3D>
#include <QQuaternion>

class Q_QUICK3DJOLTPHYSICS_EXPORT PathConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(PathConstraintPathBase *path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QVector3D pathPosition READ pathPosition WRITE setPathPosition NOTIFY pathPositionChanged)
    Q_PROPERTY(QQuaternion pathRotation READ pathRotation WRITE setPathRotation NOTIFY pathRotationChanged)
    Q_PROPERTY(float pathFraction READ pathFraction WRITE setPathFraction NOTIFY pathFractionChanged)
    Q_PROPERTY(float maxFrictionForce READ maxFrictionForce WRITE setMaxFrictionForce NOTIFY maxFrictionForceChanged)
    Q_PROPERTY(MotorSettings *positionMotorSettings READ positionMotorSettings WRITE setPositionMotorSettings NOTIFY positionMotorSettingsChanged)
    Q_PROPERTY(RotationConstraintType rotationConstraintType READ rotationConstraintType WRITE setRotationConstraintType NOTIFY rotationConstraintTypeChanged)
    Q_PROPERTY(MotorState positionMotorState READ positionMotorState WRITE setPositionMotorState NOTIFY positionMotorStateChanged)
    Q_PROPERTY(float targetVelocity READ targetVelocity WRITE setTargetVelocity NOTIFY targetVelocityChanged)
    Q_PROPERTY(float targetPathFraction READ targetPathFraction WRITE setTargetPathFraction NOTIFY targetPathFractionChanged)
    QML_NAMED_ELEMENT(PathConstraint)
public:
    explicit PathConstraint(QQuick3DNode *parent = nullptr);
    ~PathConstraint() override;

    enum class RotationConstraintType {
        Free,
        ConstrainAroundTangent,
        ConstrainAroundNormal,
        ConstrainAroundBinormal,
        ConstrainToPath,
        FullyConstrained,
    };
    Q_ENUM(RotationConstraintType)

    enum class MotorState {
        Off,
        Velocity,
        Position,
    };
    Q_ENUM(MotorState)

    PathConstraintPathBase *path() const;
    void setPath(PathConstraintPathBase *path);

    QVector3D pathPosition() const;
    void setPathPosition(const QVector3D &pathPosition);

    QQuaternion pathRotation() const;
    void setPathRotation(const QQuaternion &pathRotation);

    float pathFraction() const;
    void setPathFraction(float pathFraction);

    float maxFrictionForce() const;
    void setMaxFrictionForce(float maxFrictionForce);

    MotorSettings *positionMotorSettings() const;
    void setPositionMotorSettings(MotorSettings *settings);

    RotationConstraintType rotationConstraintType() const;
    void setRotationConstraintType(RotationConstraintType type);

    MotorState positionMotorState() const;
    void setPositionMotorState(MotorState state);

    float targetVelocity() const;
    void setTargetVelocity(float velocity);

    float targetPathFraction() const;
    void setTargetPathFraction(float fraction);

    Q_INVOKABLE float getCurrentPathFraction() const;
    Q_INVOKABLE float getTotalLambdaMotor() const;
    Q_INVOKABLE float getTotalLambdaPositionLimits() const;

signals:
    void pathChanged(PathConstraintPathBase *path);
    void pathPositionChanged(const QVector3D &pathPosition);
    void pathRotationChanged(const QQuaternion &pathRotation);
    void pathFractionChanged(float pathFraction);
    void maxFrictionForceChanged(float maxFrictionForce);
    void positionMotorSettingsChanged(MotorSettings *settings);
    void rotationConstraintTypeChanged(RotationConstraintType type);
    void positionMotorStateChanged(MotorState state);
    void targetVelocityChanged(float velocity);
    void targetPathFractionChanged(float fraction);

protected:
    void updateJoltObject() override;

private:
    void applyRuntimeMotorSettings();
    void applyRuntimePath();

    PathConstraintPathBase *m_path = nullptr;
    QVector3D m_pathPosition;
    QQuaternion m_pathRotation;
    float m_pathFraction = 0.0f;
    float m_maxFrictionForce = 0.0f;
    MotorSettings *m_positionMotorSettings = nullptr;
    QMetaObject::Connection m_pathSignalConnection;
    QMetaObject::Connection m_motorSettingsConnection;
    RotationConstraintType m_rotationConstraintType = RotationConstraintType::Free;
    MotorState m_positionMotorState = MotorState::Off;
    float m_targetVelocity = 0.0f;
    float m_targetPathFraction = 0.0f;
    JPH::PathConstraintSettings m_constraintSettings;
};

#endif // PATHCONSTRAINT_P_H
