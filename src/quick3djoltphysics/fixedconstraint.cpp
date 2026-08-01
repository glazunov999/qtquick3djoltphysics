#include "fixedconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

FixedConstraintSettings::FixedConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

bool FixedConstraintSettings::autoDetectPoint() const
{
    return m_autoDetectPoint;
}

void FixedConstraintSettings::setAutoDetectPoint(bool autoDetectPoint)
{
    if (m_autoDetectPoint == autoDetectPoint)
        return;

    m_autoDetectPoint = autoDetectPoint;
    emit autoDetectPointChanged(m_autoDetectPoint);
    emit changed();
}

QVector3D FixedConstraintSettings::point1() const
{
    return m_point1;
}

void FixedConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D FixedConstraintSettings::point2() const
{
    return m_point2;
}

void FixedConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

QVector3D FixedConstraintSettings::axisX1() const
{
    return m_axisX1;
}

void FixedConstraintSettings::setAxisX1(const QVector3D &axisX1)
{
    if (m_axisX1 == axisX1)
        return;

    m_axisX1 = axisX1;
    emit axisX1Changed(m_axisX1);
    emit changed();
}

QVector3D FixedConstraintSettings::axisX2() const
{
    return m_axisX2;
}

void FixedConstraintSettings::setAxisX2(const QVector3D &axisX2)
{
    if (m_axisX2 == axisX2)
        return;

    m_axisX2 = axisX2;
    emit axisX2Changed(m_axisX2);
    emit changed();
}

QVector3D FixedConstraintSettings::axisY1() const
{
    return m_axisY1;
}

void FixedConstraintSettings::setAxisY1(const QVector3D &axisY1)
{
    if (m_axisY1 == axisY1)
        return;

    m_axisY1 = axisY1;
    emit axisY1Changed(m_axisY1);
    emit changed();
}

QVector3D FixedConstraintSettings::axisY2() const
{
    return m_axisY2;
}

void FixedConstraintSettings::setAxisY2(const QVector3D &axisY2)
{
    if (m_axisY2 == axisY2)
        return;

    m_axisY2 = axisY2;
    emit axisY2Changed(m_axisY2);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> FixedConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::FixedConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mAutoDetectPoint = m_autoDetectPoint;
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    settings->mAxisX1 = PhysicsUtils::toJoltType(m_axisX1);
    settings->mAxisX2 = PhysicsUtils::toJoltType(m_axisX2);
    settings->mAxisY1 = PhysicsUtils::toJoltType(m_axisY1);
    settings->mAxisY2 = PhysicsUtils::toJoltType(m_axisY2);
    mapToWorld(settings, localFrame);
    return settings;
}

void FixedConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                         const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *fixed = JPH::DynamicCast<JPH::FixedConstraintSettings>(settings);
    if (fixed == nullptr)
        return;

    const QQuaternion rotation = localFrame->sceneRotation();
    mapPositionToWorld(fixed->mPoint1, localFrame);
    mapPositionToWorld(fixed->mPoint2, localFrame);
    mapDirectionToWorld(fixed->mAxisX1, rotation);
    mapDirectionToWorld(fixed->mAxisX2, rotation);
    mapDirectionToWorld(fixed->mAxisY1, rotation);
    mapDirectionToWorld(fixed->mAxisY2, rotation);
}

FixedConstraint::FixedConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new FixedConstraintSettings(this));
}

FixedConstraint::~FixedConstraint() = default;

FixedConstraintSettings *FixedConstraint::settings() const
{
    return m_settings;
}

void FixedConstraint::setSettings(FixedConstraintSettings *settings)
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

void FixedConstraint::updateJoltObject()
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
