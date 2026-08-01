#include "pointconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PointConstraintSettings::PointConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D PointConstraintSettings::point1() const
{
    return m_point1;
}

void PointConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D PointConstraintSettings::point2() const
{
    return m_point2;
}

void PointConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> PointConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::PointConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    mapToWorld(settings, localFrame);
    return settings;
}

void PointConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                         const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *point = JPH::DynamicCast<JPH::PointConstraintSettings>(settings);
    if (point == nullptr)
        return;

    mapPositionToWorld(point->mPoint1, localFrame);
    mapPositionToWorld(point->mPoint2, localFrame);
}

PointConstraint::PointConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new PointConstraintSettings(this));
}

PointConstraint::~PointConstraint() = default;

PointConstraintSettings *PointConstraint::settings() const
{
    return m_settings;
}

void PointConstraint::setSettings(PointConstraintSettings *settings)
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

void PointConstraint::updateJoltObject()
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
