#include "distanceconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

DistanceConstraint::DistanceConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

DistanceConstraint::~DistanceConstraint() = default;

QVector3D DistanceConstraint::point1() const
{
    return m_point1;
}

void DistanceConstraint::setPoint1(const QVector3D &point)
{
    if (m_point1 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'point1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_point1 = point;
    emit point1Changed(m_point1);
}

QVector3D DistanceConstraint::point2() const
{
    return m_point2;
}

void DistanceConstraint::setPoint2(const QVector3D &point)
{
    if (m_point2 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'point2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_point2 = point;
    emit point2Changed(m_point2);
}

float DistanceConstraint::minDistance() const
{
    return m_constraintSettings.mMinDistance;
}

void DistanceConstraint::setMinDistance(float minDistance)
{
    if (qFuzzyCompare(m_constraintSettings.mMinDistance, minDistance))
        return;

    m_constraintSettings.mMinDistance = minDistance;
    if (m_constraint)
        static_cast<JPH::DistanceConstraint *>(m_constraint)->SetDistance(m_constraintSettings.mMinDistance, m_constraintSettings.mMaxDistance);

    emit minDistanceChanged(minDistance);
}

float DistanceConstraint::maxDistance() const
{
    return m_constraintSettings.mMaxDistance;
}

void DistanceConstraint::setMaxDistance(float maxDistance)
{
    if (qFuzzyCompare(m_constraintSettings.mMaxDistance, maxDistance))
        return;

    m_constraintSettings.mMaxDistance = maxDistance;
    if (m_constraint)
        static_cast<JPH::DistanceConstraint *>(m_constraint)->SetDistance(m_constraintSettings.mMinDistance, m_constraintSettings.mMaxDistance);

    emit maxDistanceChanged(maxDistance);
}

SpringSettings *DistanceConstraint::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void DistanceConstraint::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
{
    if (m_limitsSpringSettings == limitsSpringSettings)
        return;

    if (m_limitsSpringSettings)
        m_limitsSpringSettings->disconnect(this);

    m_limitsSpringSettings = limitsSpringSettings;

    if (m_limitsSpringSettings) {
        QObject::connect(m_limitsSpringSettings, &SpringSettings::changed, this,
                         [this]
                         {
                             if (m_constraint != nullptr)
                                 static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());
                         });
        QObject::connect(m_limitsSpringSettings, &QObject::destroyed, this,
                         [this](QObject *obj)
                         {
                             if (m_limitsSpringSettings == obj)
                                 setLimitsSpringSettings(nullptr);
                         });
    }

    if (m_constraint != nullptr) {
        if (m_limitsSpringSettings)
            static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());
        else
            static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(JPH::SpringSettings());
    }

    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
}

void DistanceConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);

    if (m_limitsSpringSettings)
        m_constraintSettings.mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
