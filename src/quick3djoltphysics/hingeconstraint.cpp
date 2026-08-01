#include "hingeconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

HingeConstraintSettings::HingeConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D HingeConstraintSettings::point1() const
{
    return m_point1;
}

void HingeConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D HingeConstraintSettings::point2() const
{
    return m_point2;
}

void HingeConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

QVector3D HingeConstraintSettings::hingeAxis1() const
{
    return m_hingeAxis1;
}

void HingeConstraintSettings::setHingeAxis1(const QVector3D &hingeAxis1)
{
    if (m_hingeAxis1 == hingeAxis1)
        return;

    m_hingeAxis1 = hingeAxis1;
    emit hingeAxis1Changed(m_hingeAxis1);
    emit changed();
}

QVector3D HingeConstraintSettings::hingeAxis2() const
{
    return m_hingeAxis2;
}

void HingeConstraintSettings::setHingeAxis2(const QVector3D &hingeAxis2)
{
    if (m_hingeAxis2 == hingeAxis2)
        return;

    m_hingeAxis2 = hingeAxis2;
    emit hingeAxis2Changed(m_hingeAxis2);
    emit changed();
}

QVector3D HingeConstraintSettings::normalAxis1() const
{
    return m_normalAxis1;
}

void HingeConstraintSettings::setNormalAxis1(const QVector3D &normalAxis1)
{
    if (m_normalAxis1 == normalAxis1)
        return;

    m_normalAxis1 = normalAxis1;
    emit normalAxis1Changed(m_normalAxis1);
    emit changed();
}

QVector3D HingeConstraintSettings::normalAxis2() const
{
    return m_normalAxis2;
}

void HingeConstraintSettings::setNormalAxis2(const QVector3D &normalAxis2)
{
    if (m_normalAxis2 == normalAxis2)
        return;

    m_normalAxis2 = normalAxis2;
    emit normalAxis2Changed(m_normalAxis2);
    emit changed();
}

float HingeConstraintSettings::limitsMin() const
{
    return m_limitsMin;
}

void HingeConstraintSettings::setLimitsMin(float limitsMin)
{
    if (qFuzzyCompare(m_limitsMin, limitsMin))
        return;

    m_limitsMin = limitsMin;
    emit limitsMinChanged(m_limitsMin);
    emit changed();
}

float HingeConstraintSettings::limitsMax() const
{
    return m_limitsMax;
}

void HingeConstraintSettings::setLimitsMax(float limitsMax)
{
    if (qFuzzyCompare(m_limitsMax, limitsMax))
        return;

    m_limitsMax = limitsMax;
    emit limitsMaxChanged(m_limitsMax);
    emit changed();
}

SpringSettings *HingeConstraintSettings::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void HingeConstraintSettings::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
{
    if (m_limitsSpringSettings == limitsSpringSettings)
        return;

    if (m_limitsSpringSettings)
        m_limitsSpringSettings->disconnect(this);

    m_limitsSpringSettings = limitsSpringSettings;

    if (m_limitsSpringSettings) {
        connect(m_limitsSpringSettings, &SpringSettings::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_limitsSpringSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_limitsSpringSettings == obj)
                setLimitsSpringSettings(nullptr);
        });
    }

    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
    emit changed();
}

float HingeConstraintSettings::maxFrictionTorque() const
{
    return m_maxFrictionTorque;
}

void HingeConstraintSettings::setMaxFrictionTorque(float maxFrictionTorque)
{
    if (qFuzzyCompare(m_maxFrictionTorque, maxFrictionTorque))
        return;

    m_maxFrictionTorque = maxFrictionTorque;
    emit maxFrictionTorqueChanged(m_maxFrictionTorque);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> HingeConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::HingeConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    settings->mHingeAxis1 = PhysicsUtils::toJoltType(m_hingeAxis1);
    settings->mHingeAxis2 = PhysicsUtils::toJoltType(m_hingeAxis2);
    settings->mNormalAxis1 = PhysicsUtils::toJoltType(m_normalAxis1);
    settings->mNormalAxis2 = PhysicsUtils::toJoltType(m_normalAxis2);
    settings->mLimitsMin = qDegreesToRadians(m_limitsMin);
    settings->mLimitsMax = qDegreesToRadians(m_limitsMax);
    if (m_limitsSpringSettings)
        settings->mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();
    settings->mMaxFrictionTorque = m_maxFrictionTorque;
    mapToWorld(settings, localFrame);
    return settings;
}

void HingeConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                         const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *hinge = JPH::DynamicCast<JPH::HingeConstraintSettings>(settings);
    if (hinge == nullptr)
        return;

    const QQuaternion rotation = localFrame->sceneRotation();
    mapPositionToWorld(hinge->mPoint1, localFrame);
    mapPositionToWorld(hinge->mPoint2, localFrame);
    mapDirectionToWorld(hinge->mHingeAxis1, rotation);
    mapDirectionToWorld(hinge->mHingeAxis2, rotation);
    mapDirectionToWorld(hinge->mNormalAxis1, rotation);
    mapDirectionToWorld(hinge->mNormalAxis2, rotation);
}

HingeConstraint::HingeConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new HingeConstraintSettings(this));
}

HingeConstraint::~HingeConstraint() = default;

HingeConstraintSettings *HingeConstraint::settings() const
{
    return m_settings;
}

void HingeConstraint::setSettings(HingeConstraintSettings *settings)
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

void HingeConstraint::updateJoltObject()
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
