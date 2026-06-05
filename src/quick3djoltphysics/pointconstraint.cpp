#include "pointconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PointConstraint::PointConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

PointConstraint::~PointConstraint() = default;

QVector3D PointConstraint::point1() const
{
    return m_point1;
}

void PointConstraint::setPoint1(const QVector3D &point)
{
    if (m_point1 == point)
        return;

    m_point1 = point;

    if (m_constraint) {
        auto *constraint = static_cast<JPH::PointConstraint *>(m_constraint);
        constraint->SetPoint1(static_cast<JPH::EConstraintSpace>(m_space), PhysicsUtils::toJoltType(m_point1));
    }

    emit point1Changed(m_point1);
}

QVector3D PointConstraint::point2() const
{
    return m_point2;
}

void PointConstraint::setPoint2(const QVector3D &point)
{
    if (m_point2 == point)
        return;

    m_point2 = point;

    if (m_constraint) {
        auto *constraint = static_cast<JPH::PointConstraint *>(m_constraint);
        constraint->SetPoint2(static_cast<JPH::EConstraintSpace>(m_space), PhysicsUtils::toJoltType(m_point2));
    }

    emit point2Changed(m_point2);
}

void PointConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
