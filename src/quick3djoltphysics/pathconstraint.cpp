#include "pathconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PathConstraintSettings::PathConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

PathConstraintPathBase *PathConstraintSettings::path() const
{
    return m_path;
}

void PathConstraintSettings::setPath(PathConstraintPathBase *path)
{
    if (m_path == path)
        return;

    if (m_path)
        m_path->disconnect(this);

    m_path = path;

    if (m_path) {
        connect(m_path, &PathConstraintPathBase::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_path, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_path == obj)
                setPath(nullptr);
        });
    }

    emit pathChanged(m_path);
    emit changed();
}

QVector3D PathConstraintSettings::pathPosition() const
{
    return m_pathPosition;
}

void PathConstraintSettings::setPathPosition(const QVector3D &pathPosition)
{
    if (m_pathPosition == pathPosition)
        return;

    m_pathPosition = pathPosition;
    emit pathPositionChanged(m_pathPosition);
    emit changed();
}

QQuaternion PathConstraintSettings::pathRotation() const
{
    return m_pathRotation;
}

void PathConstraintSettings::setPathRotation(const QQuaternion &pathRotation)
{
    if (m_pathRotation == pathRotation)
        return;

    m_pathRotation = pathRotation;
    emit pathRotationChanged(m_pathRotation);
    emit changed();
}

float PathConstraintSettings::pathFraction() const
{
    return m_pathFraction;
}

void PathConstraintSettings::setPathFraction(float pathFraction)
{
    if (qFuzzyCompare(m_pathFraction, pathFraction))
        return;

    m_pathFraction = pathFraction;
    emit pathFractionChanged(m_pathFraction);
    emit changed();
}

float PathConstraintSettings::maxFrictionForce() const
{
    return m_maxFrictionForce;
}

void PathConstraintSettings::setMaxFrictionForce(float maxFrictionForce)
{
    if (qFuzzyCompare(m_maxFrictionForce, maxFrictionForce))
        return;

    m_maxFrictionForce = maxFrictionForce;
    emit maxFrictionForceChanged(m_maxFrictionForce);
    emit changed();
}

MotorSettings *PathConstraintSettings::positionMotorSettings() const
{
    return m_positionMotorSettings;
}

void PathConstraintSettings::setPositionMotorSettings(MotorSettings *positionMotorSettings)
{
    if (m_positionMotorSettings == positionMotorSettings)
        return;

    if (m_positionMotorSettings)
        m_positionMotorSettings->disconnect(this);

    m_positionMotorSettings = positionMotorSettings;

    if (m_positionMotorSettings) {
        connect(m_positionMotorSettings, &MotorSettings::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_positionMotorSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_positionMotorSettings == obj)
                setPositionMotorSettings(nullptr);
        });
    }

    emit positionMotorSettingsChanged(m_positionMotorSettings);
    emit changed();
}

PathConstraintSettings::RotationConstraintType PathConstraintSettings::rotationConstraintType() const
{
    return m_rotationConstraintType;
}

void PathConstraintSettings::setRotationConstraintType(RotationConstraintType rotationConstraintType)
{
    if (m_rotationConstraintType == rotationConstraintType)
        return;

    m_rotationConstraintType = rotationConstraintType;
    emit rotationConstraintTypeChanged(m_rotationConstraintType);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> PathConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::PathConstraintSettings;
    applyBaseSettings(*settings);
    if (m_path)
        settings->mPath = m_path->createJoltPath();
    settings->mPathPosition = PhysicsUtils::toJoltType(m_pathPosition);
    settings->mPathRotation = PhysicsUtils::toJoltType(m_pathRotation);
    settings->mPathFraction = m_pathFraction;
    settings->mMaxFrictionForce = m_maxFrictionForce;
    if (m_positionMotorSettings)
        settings->mPositionMotorSettings = m_positionMotorSettings->getJoltMotorSettings();
    settings->mRotationConstraintType = static_cast<JPH::EPathRotationConstraintType>(m_rotationConstraintType);
    mapToWorld(settings, localFrame);
    return settings;
}

void PathConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                        const QQuick3DNode *localFrame) const
{
    Q_UNUSED(settings);
    Q_UNUSED(localFrame);
}

PathConstraint::PathConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new PathConstraintSettings(this));
}

PathConstraint::~PathConstraint() = default;

PathConstraintSettings *PathConstraint::settings() const
{
    return m_settings;
}

void PathConstraint::setSettings(PathConstraintSettings *settings)
{
    if (m_settings == settings)
        return;

    if (m_settings != nullptr)
        m_settings->disconnect(this);

    m_settings = settings;

    if (m_settings != nullptr) {
        QObject::connect(m_settings, &AbstractPhysicsConstraintSettings::changed, this,
                         [this] { updateJoltObject(); });
        QObject::connect(m_settings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_settings == obj)
                setSettings(nullptr);
        });
    }

    updateJoltObject();
    emit settingsChanged(m_settings);
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

void PathConstraint::applyRuntimeMotorState()
{
    if (!m_constraint)
        return;

    auto *pc = static_cast<JPH::PathConstraint *>(m_constraint);

    if (m_positionMotorState != MotorState::Off)
        pc->SetPositionMotorState(static_cast<JPH::EMotorState>(m_positionMotorState));

    if (m_positionMotorState == MotorState::Velocity)
        pc->SetTargetVelocity(m_targetVelocity);
    else if (m_positionMotorState == MotorState::Position)
        pc->SetTargetPathFraction(m_targetPathFraction);
}

void PathConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady() || m_settings == nullptr)
        return;

    PathConstraintPathBase *path = m_settings->path();
    if (!path) {
        if (m_constraint) {
            m_jolt->RemoveConstraint(m_constraint);
            m_constraint = nullptr;
        }
        return;
    }

    if (path->pointCount() < 2) {
        if (m_constraint) {
            m_jolt->RemoveConstraint(m_constraint);
            m_constraint = nullptr;
        }
        return;
    }

    if (!path->createJoltPath()) {
        qWarning() << "PathConstraint: failed to create Jolt path";
        return;
    }

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    const JPH::Ref<JPH::TwoBodyConstraintSettings> settings =
            m_settings->createJoltTwoBodyConstraintSettings();
    m_constraint = settings->Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);

    applyRuntimeMotorState();
}
