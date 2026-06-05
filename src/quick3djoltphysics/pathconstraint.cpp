#include "pathconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PathConstraint::PathConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

PathConstraint::~PathConstraint() = default;

PathConstraintPathBase *PathConstraint::path() const
{
    return m_path;
}

void PathConstraint::setPath(PathConstraintPathBase *path)
{
    if (m_path == path)
        return;

    if (m_pathSignalConnection)
        QObject::disconnect(m_pathSignalConnection);
    m_pathSignalConnection = {};

    m_path = path;
    if (m_path) {
        m_pathSignalConnection = QObject::connect(m_path, &PathConstraintPathBase::changed, this,
                                                  [this] {
                                                      if (m_constraint)
                                                          applyRuntimePath();
                                                      else
                                                          updateJoltObject();
                                                  });
        QObject::connect(m_path, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_path == obj)
                setPath(nullptr);
        });
    }

    if (m_constraint)
        applyRuntimePath();
    else
        updateJoltObject();

    emit pathChanged(m_path);
}

QVector3D PathConstraint::pathPosition() const
{
    return m_pathPosition;
}

void PathConstraint::setPathPosition(const QVector3D &pathPosition)
{
    if (m_pathPosition == pathPosition)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'pathPosition' after constraint is initialized will "
                      "have no effect";
        return;
    }

    m_pathPosition = pathPosition;
    emit pathPositionChanged(m_pathPosition);
}

QQuaternion PathConstraint::pathRotation() const
{
    return m_pathRotation;
}

void PathConstraint::setPathRotation(const QQuaternion &pathRotation)
{
    if (m_pathRotation == pathRotation)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'pathRotation' after constraint is initialized will "
                      "have no effect";
        return;
    }

    m_pathRotation = pathRotation;
    emit pathRotationChanged(m_pathRotation);
}

float PathConstraint::pathFraction() const
{
    return m_pathFraction;
}

void PathConstraint::setPathFraction(float pathFraction)
{
    if (qFuzzyCompare(m_pathFraction, pathFraction))
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'pathFraction' after constraint is initialized will "
                      "have no effect";
        return;
    }

    m_pathFraction = pathFraction;
    emit pathFractionChanged(m_pathFraction);
}

float PathConstraint::maxFrictionForce() const
{
    return m_maxFrictionForce;
}

void PathConstraint::setMaxFrictionForce(float maxFrictionForce)
{
    if (qFuzzyCompare(m_maxFrictionForce, maxFrictionForce))
        return;

    m_maxFrictionForce = maxFrictionForce;

    if (m_constraint)
        static_cast<JPH::PathConstraint *>(m_constraint)->SetMaxFrictionForce(m_maxFrictionForce);

    emit maxFrictionForceChanged(m_maxFrictionForce);
}

MotorSettings *PathConstraint::positionMotorSettings() const
{
    return m_positionMotorSettings;
}

void PathConstraint::setPositionMotorSettings(MotorSettings *settings)
{
    if (m_positionMotorSettings == settings)
        return;

    if (m_motorSettingsConnection)
        QObject::disconnect(m_motorSettingsConnection);
    m_motorSettingsConnection = {};

    m_positionMotorSettings = settings;
    if (m_positionMotorSettings) {
        m_motorSettingsConnection = QObject::connect(
            m_positionMotorSettings, &MotorSettings::changed, this,
            [this] { applyRuntimeMotorSettings(); });
        QObject::connect(m_positionMotorSettings, &QObject::destroyed, this,
                         [this](QObject *obj) {
                             if (m_positionMotorSettings == obj)
                                 setPositionMotorSettings(nullptr);
                         });
    }

    applyRuntimeMotorSettings();
    emit positionMotorSettingsChanged(m_positionMotorSettings);
}

PathConstraint::RotationConstraintType PathConstraint::rotationConstraintType() const
{
    return m_rotationConstraintType;
}

void PathConstraint::setRotationConstraintType(RotationConstraintType type)
{
    if (m_rotationConstraintType == type)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'rotationConstraintType' after constraint is "
                      "initialized will have no effect";
        return;
    }

    m_rotationConstraintType = type;
    emit rotationConstraintTypeChanged(m_rotationConstraintType);
}

PathConstraint::MotorState PathConstraint::positionMotorState() const
{
    return m_positionMotorState;
}

void PathConstraint::setPositionMotorState(MotorState state)
{
    if (m_positionMotorState == state)
        return;

    m_positionMotorState = state;

    if (m_constraint)
        static_cast<JPH::PathConstraint *>(m_constraint)->SetPositionMotorState(
            static_cast<JPH::EMotorState>(m_positionMotorState));

    emit positionMotorStateChanged(m_positionMotorState);
}

float PathConstraint::targetVelocity() const
{
    return m_targetVelocity;
}

void PathConstraint::setTargetVelocity(float velocity)
{
    if (qFuzzyCompare(m_targetVelocity, velocity))
        return;

    m_targetVelocity = velocity;

    if (m_constraint)
        static_cast<JPH::PathConstraint *>(m_constraint)->SetTargetVelocity(m_targetVelocity);

    emit targetVelocityChanged(m_targetVelocity);
}

float PathConstraint::targetPathFraction() const
{
    return m_targetPathFraction;
}

void PathConstraint::setTargetPathFraction(float fraction)
{
    if (qFuzzyCompare(m_targetPathFraction, fraction))
        return;

    m_targetPathFraction = fraction;

    if (m_constraint)
        static_cast<JPH::PathConstraint *>(m_constraint)->SetTargetPathFraction(
            m_targetPathFraction);

    emit targetPathFractionChanged(m_targetPathFraction);
}

float PathConstraint::getCurrentPathFraction() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::PathConstraint *>(m_constraint)->GetPathFraction();
}

float PathConstraint::getTotalLambdaMotor() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::PathConstraint *>(m_constraint)->GetTotalLambdaMotor();
}

float PathConstraint::getTotalLambdaPositionLimits() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::PathConstraint *>(m_constraint)->GetTotalLambdaPositionLimits();
}

void PathConstraint::applyRuntimeMotorSettings()
{
    if (!m_constraint || !m_positionMotorSettings)
        return;

    static_cast<JPH::PathConstraint *>(m_constraint)->GetPositionMotorSettings() =
        m_positionMotorSettings->getJoltMotorSettings();
}

void PathConstraint::applyRuntimePath()
{
    if (!m_constraint || !m_path)
        return;

    if (m_path->pointCount() < 2)
        return;

    auto joltPath = m_path->createJoltPath();
    if (!joltPath)
        return;

    static_cast<JPH::PathConstraint *>(m_constraint)->SetPath(joltPath, m_pathFraction);
}

void PathConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (!m_path) {
        return;
    }

    if (m_path->pointCount() < 2) {
        if (m_constraint) {
            m_jolt->RemoveConstraint(m_constraint);
            m_constraint = nullptr;
        }
        return;
    }

    auto joltPath = m_path->createJoltPath();
    if (!joltPath) {
        qWarning() << "PathConstraint: failed to create Jolt path";
        return;
    }

    if (m_constraint) {
        m_jolt->RemoveConstraint(m_constraint);
        m_constraint = nullptr;
    }

    m_constraintSettings.mPath = joltPath;
    m_constraintSettings.mPathPosition = PhysicsUtils::toJoltType(m_pathPosition);
    m_constraintSettings.mPathRotation = PhysicsUtils::toJoltType(m_pathRotation);
    m_constraintSettings.mPathFraction = m_pathFraction;
    m_constraintSettings.mMaxFrictionForce = m_maxFrictionForce;
    m_constraintSettings.mRotationConstraintType =
        static_cast<JPH::EPathRotationConstraintType>(m_rotationConstraintType);

    if (m_positionMotorSettings)
        m_constraintSettings.mPositionMotorSettings =
            m_positionMotorSettings->getJoltMotorSettings();

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);

    auto *pc = static_cast<JPH::PathConstraint *>(m_constraint);

    if (m_positionMotorState != MotorState::Off)
        pc->SetPositionMotorState(static_cast<JPH::EMotorState>(m_positionMotorState));

    if (m_positionMotorState == MotorState::Velocity)
        pc->SetTargetVelocity(m_targetVelocity);
    else if (m_positionMotorState == MotorState::Position)
        pc->SetTargetPathFraction(m_targetPathFraction);
}
