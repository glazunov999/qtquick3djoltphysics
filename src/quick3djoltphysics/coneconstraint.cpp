#include "coneconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

ConeConstraintSettings::ConeConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D ConeConstraintSettings::point1() const
{
    return m_point1;
}

void ConeConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D ConeConstraintSettings::point2() const
{
    return m_point2;
}

void ConeConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

QVector3D ConeConstraintSettings::twistAxis1() const
{
    return m_twistAxis1;
}

void ConeConstraintSettings::setTwistAxis1(const QVector3D &twistAxis1)
{
    if (m_twistAxis1 == twistAxis1)
        return;

    m_twistAxis1 = twistAxis1;
    emit twistAxis1Changed(m_twistAxis1);
    emit changed();
}

QVector3D ConeConstraintSettings::twistAxis2() const
{
    return m_twistAxis2;
}

void ConeConstraintSettings::setTwistAxis2(const QVector3D &twistAxis2)
{
    if (m_twistAxis2 == twistAxis2)
        return;

    m_twistAxis2 = twistAxis2;
    emit twistAxis2Changed(m_twistAxis2);
    emit changed();
}

float ConeConstraintSettings::halfConeAngle() const
{
    return m_halfConeAngle;
}

void ConeConstraintSettings::setHalfConeAngle(float halfConeAngle)
{
    if (qFuzzyCompare(m_halfConeAngle, halfConeAngle))
        return;

    m_halfConeAngle = halfConeAngle;
    emit halfConeAngleChanged(m_halfConeAngle);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> ConeConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::ConeConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    settings->mTwistAxis1 = PhysicsUtils::toJoltType(m_twistAxis1);
    settings->mTwistAxis2 = PhysicsUtils::toJoltType(m_twistAxis2);
    settings->mHalfConeAngle = qDegreesToRadians(m_halfConeAngle);
    mapToWorld(settings, localFrame);
    return settings;
}

void ConeConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                        const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *cone = JPH::DynamicCast<JPH::ConeConstraintSettings>(settings);
    if (cone == nullptr)
        return;

    const QQuaternion rotation = localFrame->sceneRotation();
    mapPositionToWorld(cone->mPoint1, localFrame);
    mapPositionToWorld(cone->mPoint2, localFrame);
    mapDirectionToWorld(cone->mTwistAxis1, rotation);
    mapDirectionToWorld(cone->mTwistAxis2, rotation);
}

ConeConstraint::ConeConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new ConeConstraintSettings(this));
}

ConeConstraint::~ConeConstraint() = default;

ConeConstraintSettings *ConeConstraint::settings() const
{
    return m_settings;
}

void ConeConstraint::setSettings(ConeConstraintSettings *settings)
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

void ConeConstraint::updateJoltObject()
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
