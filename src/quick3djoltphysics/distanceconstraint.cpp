#include "distanceconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

DistanceConstraintSettings::DistanceConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D DistanceConstraintSettings::point1() const
{
    return m_point1;
}

void DistanceConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D DistanceConstraintSettings::point2() const
{
    return m_point2;
}

void DistanceConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

float DistanceConstraintSettings::minDistance() const
{
    return m_minDistance;
}

void DistanceConstraintSettings::setMinDistance(float minDistance)
{
    if (qFuzzyCompare(m_minDistance, minDistance))
        return;

    m_minDistance = minDistance;
    emit minDistanceChanged(m_minDistance);
    emit changed();
}

float DistanceConstraintSettings::maxDistance() const
{
    return m_maxDistance;
}

void DistanceConstraintSettings::setMaxDistance(float maxDistance)
{
    if (qFuzzyCompare(m_maxDistance, maxDistance))
        return;

    m_maxDistance = maxDistance;
    emit maxDistanceChanged(m_maxDistance);
    emit changed();
}

SpringSettings *DistanceConstraintSettings::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void DistanceConstraintSettings::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
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

JPH::Ref<JPH::TwoBodyConstraintSettings> DistanceConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::DistanceConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    settings->mMinDistance = m_minDistance;
    settings->mMaxDistance = m_maxDistance;
    if (m_limitsSpringSettings)
        settings->mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();
    mapToWorld(settings, localFrame);
    return settings;
}

void DistanceConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                            const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *distance = JPH::DynamicCast<JPH::DistanceConstraintSettings>(settings);
    if (distance == nullptr)
        return;

    mapPositionToWorld(distance->mPoint1, localFrame);
    mapPositionToWorld(distance->mPoint2, localFrame);
}

DistanceConstraint::DistanceConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new DistanceConstraintSettings(this));
}

DistanceConstraint::~DistanceConstraint() = default;

DistanceConstraintSettings *DistanceConstraint::settings() const
{
    return m_settings;
}

void DistanceConstraint::setSettings(DistanceConstraintSettings *settings)
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

void DistanceConstraint::updateJoltObject()
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
