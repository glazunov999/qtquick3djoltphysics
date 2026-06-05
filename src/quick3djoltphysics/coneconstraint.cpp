#include "coneconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

ConeConstraint::ConeConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

ConeConstraint::~ConeConstraint() = default;

QVector3D ConeConstraint::point1() const
{
    return m_point1;
}

void ConeConstraint::setPoint1(const QVector3D &point)
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

QVector3D ConeConstraint::point2() const
{
    return m_point2;
}

void ConeConstraint::setPoint2(const QVector3D &point)
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

QVector3D ConeConstraint::twistAxis1() const
{
    return m_twistAxis1;
}

void ConeConstraint::setTwistAxis1(const QVector3D &twistAxis1)
{
    if (m_twistAxis1 == twistAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'twistAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_twistAxis1 = twistAxis1;
    emit twistAxis1Changed(m_twistAxis1);
}

QVector3D ConeConstraint::twistAxis2() const
{
    return m_twistAxis2;
}

void ConeConstraint::setTwistAxis2(const QVector3D &twistAxis2)
{
    if (m_twistAxis2 == twistAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'twistAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_twistAxis2 = twistAxis2;
    emit twistAxis2Changed(m_twistAxis2);
}

float ConeConstraint::halfConeAngle() const
{
    return m_halfConeAngle;
}

void ConeConstraint::setHalfConeAngle(float halfConeAngle)
{
    if (qFuzzyCompare(m_halfConeAngle, halfConeAngle))
        return;

    m_halfConeAngle = halfConeAngle;

    if (m_constraint)
        static_cast<JPH::ConeConstraint *>(m_constraint)->SetHalfConeAngle(qDegreesToRadians(m_halfConeAngle));

    emit halfConeAngleChanged(m_halfConeAngle);
}

void ConeConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraintSettings.mTwistAxis1 = PhysicsUtils::toJoltType(m_twistAxis1);
    m_constraintSettings.mTwistAxis2 = PhysicsUtils::toJoltType(m_twistAxis2);
    m_constraintSettings.mHalfConeAngle = qDegreesToRadians(m_halfConeAngle);

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
