#include "swingtwistconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

SwingTwistConstraintSettings::SwingTwistConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D SwingTwistConstraintSettings::position1() const
{
    return m_position1;
}

void SwingTwistConstraintSettings::setPosition1(const QVector3D &position1)
{
    if (m_position1 == position1)
        return;

    m_position1 = position1;
    emit position1Changed(m_position1);
    emit changed();
}

QVector3D SwingTwistConstraintSettings::position2() const
{
    return m_position2;
}

void SwingTwistConstraintSettings::setPosition2(const QVector3D &position2)
{
    if (m_position2 == position2)
        return;

    m_position2 = position2;
    emit position2Changed(m_position2);
    emit changed();
}

QVector3D SwingTwistConstraintSettings::twistAxis1() const
{
    return m_twistAxis1;
}

void SwingTwistConstraintSettings::setTwistAxis1(const QVector3D &twistAxis1)
{
    if (m_twistAxis1 == twistAxis1)
        return;

    m_twistAxis1 = twistAxis1;
    emit twistAxis1Changed(m_twistAxis1);
    emit changed();
}

QVector3D SwingTwistConstraintSettings::twistAxis2() const
{
    return m_twistAxis2;
}

void SwingTwistConstraintSettings::setTwistAxis2(const QVector3D &twistAxis2)
{
    if (m_twistAxis2 == twistAxis2)
        return;

    m_twistAxis2 = twistAxis2;
    emit twistAxis2Changed(m_twistAxis2);
    emit changed();
}

QVector3D SwingTwistConstraintSettings::planeAxis1() const
{
    return m_planeAxis1;
}

void SwingTwistConstraintSettings::setPlaneAxis1(const QVector3D &planeAxis1)
{
    if (m_planeAxis1 == planeAxis1)
        return;

    m_planeAxis1 = planeAxis1;
    emit planeAxis1Changed(m_planeAxis1);
    emit changed();
}

QVector3D SwingTwistConstraintSettings::planeAxis2() const
{
    return m_planeAxis2;
}

void SwingTwistConstraintSettings::setPlaneAxis2(const QVector3D &planeAxis2)
{
    if (m_planeAxis2 == planeAxis2)
        return;

    m_planeAxis2 = planeAxis2;
    emit planeAxis2Changed(m_planeAxis2);
    emit changed();
}

SwingTwistConstraintSettings::SwingType SwingTwistConstraintSettings::swingType() const
{
    return m_swingType;
}

void SwingTwistConstraintSettings::setSwingType(SwingType swingType)
{
    if (m_swingType == swingType)
        return;

    m_swingType = swingType;
    emit swingTypeChanged(m_swingType);
    emit changed();
}

float SwingTwistConstraintSettings::normalHalfConeAngle() const
{
    return m_normalHalfConeAngle;
}

void SwingTwistConstraintSettings::setNormalHalfConeAngle(float normalHalfConeAngle)
{
    if (qFuzzyCompare(m_normalHalfConeAngle, normalHalfConeAngle))
        return;

    m_normalHalfConeAngle = normalHalfConeAngle;
    emit normalHalfConeAngleChanged(m_normalHalfConeAngle);
    emit changed();
}

float SwingTwistConstraintSettings::planeHalfConeAngle() const
{
    return m_planeHalfConeAngle;
}

void SwingTwistConstraintSettings::setPlaneHalfConeAngle(float planeHalfConeAngle)
{
    if (qFuzzyCompare(m_planeHalfConeAngle, planeHalfConeAngle))
        return;

    m_planeHalfConeAngle = planeHalfConeAngle;
    emit planeHalfConeAngleChanged(m_planeHalfConeAngle);
    emit changed();
}

float SwingTwistConstraintSettings::twistMinAngle() const
{
    return m_twistMinAngle;
}

void SwingTwistConstraintSettings::setTwistMinAngle(float twistMinAngle)
{
    if (qFuzzyCompare(m_twistMinAngle, twistMinAngle))
        return;

    m_twistMinAngle = twistMinAngle;
    emit twistMinAngleChanged(m_twistMinAngle);
    emit changed();
}

float SwingTwistConstraintSettings::twistMaxAngle() const
{
    return m_twistMaxAngle;
}

void SwingTwistConstraintSettings::setTwistMaxAngle(float twistMaxAngle)
{
    if (qFuzzyCompare(m_twistMaxAngle, twistMaxAngle))
        return;

    m_twistMaxAngle = twistMaxAngle;
    emit twistMaxAngleChanged(m_twistMaxAngle);
    emit changed();
}

float SwingTwistConstraintSettings::maxFrictionTorque() const
{
    return m_maxFrictionTorque;
}

void SwingTwistConstraintSettings::setMaxFrictionTorque(float maxFrictionTorque)
{
    if (qFuzzyCompare(m_maxFrictionTorque, maxFrictionTorque))
        return;

    m_maxFrictionTorque = maxFrictionTorque;
    emit maxFrictionTorqueChanged(m_maxFrictionTorque);
    emit changed();
}

MotorSettings *SwingTwistConstraintSettings::swingMotorSettings() const
{
    return m_swingMotorSettings;
}

void SwingTwistConstraintSettings::setSwingMotorSettings(MotorSettings *swingMotorSettings)
{
    if (m_swingMotorSettings == swingMotorSettings)
        return;

    if (m_swingMotorSettings)
        m_swingMotorSettings->disconnect(this);

    m_swingMotorSettings = swingMotorSettings;

    if (m_swingMotorSettings) {
        connect(m_swingMotorSettings, &MotorSettings::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_swingMotorSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_swingMotorSettings == obj)
                setSwingMotorSettings(nullptr);
        });
    }

    emit swingMotorSettingsChanged(m_swingMotorSettings);
    emit changed();
}

MotorSettings *SwingTwistConstraintSettings::twistMotorSettings() const
{
    return m_twistMotorSettings;
}

void SwingTwistConstraintSettings::setTwistMotorSettings(MotorSettings *twistMotorSettings)
{
    if (m_twistMotorSettings == twistMotorSettings)
        return;

    if (m_twistMotorSettings)
        m_twistMotorSettings->disconnect(this);

    m_twistMotorSettings = twistMotorSettings;

    if (m_twistMotorSettings) {
        connect(m_twistMotorSettings, &MotorSettings::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_twistMotorSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_twistMotorSettings == obj)
                setTwistMotorSettings(nullptr);
        });
    }

    emit twistMotorSettingsChanged(m_twistMotorSettings);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> SwingTwistConstraintSettings::createJoltTwoBodyConstraintSettings(
        const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::SwingTwistConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPosition1 = PhysicsUtils::toJoltType(m_position1);
    settings->mPosition2 = PhysicsUtils::toJoltType(m_position2);
    settings->mTwistAxis1 = PhysicsUtils::toJoltType(m_twistAxis1);
    settings->mTwistAxis2 = PhysicsUtils::toJoltType(m_twistAxis2);
    settings->mPlaneAxis1 = PhysicsUtils::toJoltType(m_planeAxis1);
    settings->mPlaneAxis2 = PhysicsUtils::toJoltType(m_planeAxis2);
    settings->mSwingType = static_cast<JPH::ESwingType>(m_swingType);
    settings->mNormalHalfConeAngle = qDegreesToRadians(m_normalHalfConeAngle);
    settings->mPlaneHalfConeAngle = qDegreesToRadians(m_planeHalfConeAngle);
    settings->mTwistMinAngle = qDegreesToRadians(m_twistMinAngle);
    settings->mTwistMaxAngle = qDegreesToRadians(m_twistMaxAngle);
    settings->mMaxFrictionTorque = m_maxFrictionTorque;
    if (m_swingMotorSettings)
        settings->mSwingMotorSettings = m_swingMotorSettings->getJoltMotorSettings();
    if (m_twistMotorSettings)
        settings->mTwistMotorSettings = m_twistMotorSettings->getJoltMotorSettings();
    mapToWorld(settings, localFrame);
    return settings;
}

void SwingTwistConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                              const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *swingTwist = JPH::DynamicCast<JPH::SwingTwistConstraintSettings>(settings);
    if (swingTwist == nullptr)
        return;

    const QQuaternion rotation = localFrame->sceneRotation();
    mapPositionToWorld(swingTwist->mPosition1, localFrame);
    mapPositionToWorld(swingTwist->mPosition2, localFrame);
    mapDirectionToWorld(swingTwist->mTwistAxis1, rotation);
    mapDirectionToWorld(swingTwist->mTwistAxis2, rotation);
    mapDirectionToWorld(swingTwist->mPlaneAxis1, rotation);
    mapDirectionToWorld(swingTwist->mPlaneAxis2, rotation);
}

SwingTwistConstraint::SwingTwistConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new SwingTwistConstraintSettings(this));
}

SwingTwistConstraint::~SwingTwistConstraint() = default;

SwingTwistConstraintSettings *SwingTwistConstraint::settings() const
{
    return m_settings;
}

void SwingTwistConstraint::setSettings(SwingTwistConstraintSettings *settings)
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

void SwingTwistConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady() || m_settings == nullptr)
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    const JPH::Ref<JPH::TwoBodyConstraintSettings> settings =
            m_settings->createJoltTwoBodyConstraintSettings();
    m_constraint = settings->Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
