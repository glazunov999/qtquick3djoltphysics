#include "fixedconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

FixedConstraint::FixedConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

FixedConstraint::~FixedConstraint() = default;

QVector3D FixedConstraint::point1() const
{
    return m_point1;
}

void FixedConstraint::setPoint1(const QVector3D &point)
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

QVector3D FixedConstraint::point2() const
{
    return m_point2;
}

void FixedConstraint::setPoint2(const QVector3D &point)
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

QVector3D FixedConstraint::axisX1() const
{
    return m_axisX1;
}

void FixedConstraint::setAxisX1(const QVector3D &axisX1)
{
    if (m_axisX1 == axisX1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisX1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisX1 = axisX1;
    emit axisX1Changed(m_axisX1);
}

QVector3D FixedConstraint::axisX2() const
{
    return m_axisX2;
}

void FixedConstraint::setAxisX2(const QVector3D &axisX2)
{
    if (m_axisX2 == axisX2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisX2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisX2 = axisX2;
    emit axisX2Changed(m_axisX2);
}

QVector3D FixedConstraint::axisY1() const
{
    return m_axisY1;
}

void FixedConstraint::setAxisY1(const QVector3D &axisY1)
{
    if (m_axisY1 == axisY1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisY1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisY1 = axisY1;
    emit axisY1Changed(m_axisY1);
}

QVector3D FixedConstraint::axisY2() const
{
    return m_axisY2;
}

void FixedConstraint::setAxisY2(const QVector3D &axisY2)
{
    if (m_axisY2 == axisY2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisY2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisY2 = axisY2;
    emit axisY2Changed(m_axisY2);
}

void FixedConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraintSettings.mAxisX1 = PhysicsUtils::toJoltType(m_axisX1);
    m_constraintSettings.mAxisX2 = PhysicsUtils::toJoltType(m_axisX2);
    m_constraintSettings.mAxisY1 = PhysicsUtils::toJoltType(m_axisY1);
    m_constraintSettings.mAxisY2 = PhysicsUtils::toJoltType(m_axisY2);

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
