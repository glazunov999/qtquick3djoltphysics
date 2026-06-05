#include "gearconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

GearConstraint::GearConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

GearConstraint::~GearConstraint() = default;

QVector3D GearConstraint::hingeAxis1() const
{
    return m_hingeAxis1;
}

void GearConstraint::setHingeAxis1(const QVector3D &hingeAxis1)
{
    if (m_hingeAxis1 == hingeAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'hingeAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_hingeAxis1 = hingeAxis1;
    emit hingeAxis1Changed(m_hingeAxis1);
}

QVector3D GearConstraint::hingeAxis2() const
{
    return m_hingeAxis2;
}

void GearConstraint::setHingeAxis2(const QVector3D &hingeAxis2)
{
    if (m_hingeAxis2 == hingeAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'hingeAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_hingeAxis2 = hingeAxis2;
    emit hingeAxis2Changed(m_hingeAxis2);
}

float GearConstraint::ratio() const
{
    return m_ratio;
}

void GearConstraint::setRatio(float ratio)
{
    if (qFuzzyCompare(m_ratio, ratio))
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'ratio' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_ratio = ratio;
    emit ratioChanged(m_ratio);
}

HingeConstraint *GearConstraint::gear1Constraint() const
{
    return m_gear1Constraint;
}

void GearConstraint::setGear1Constraint(HingeConstraint *constraint)
{
    if (m_gear1Constraint == constraint)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &GearConstraint::setGear1Constraint, constraint,
                                         m_gear1Constraint);
    if (m_gear1Body1SignalConnection)
        QObject::disconnect(m_gear1Body1SignalConnection);
    if (m_gear1Body2SignalConnection)
        QObject::disconnect(m_gear1Body2SignalConnection);
    m_gear1Body1SignalConnection = {};
    m_gear1Body2SignalConnection = {};

    m_gear1Constraint = constraint;
    watchGearHinge(m_gear1Constraint, m_gear1Body1SignalConnection, m_gear1Body2SignalConnection);
    linkGearConstraints();

    emit gear1ConstraintChanged(m_gear1Constraint);
}

HingeConstraint *GearConstraint::gear2Constraint() const
{
    return m_gear2Constraint;
}

void GearConstraint::setGear2Constraint(HingeConstraint *constraint)
{
    if (m_gear2Constraint == constraint)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &GearConstraint::setGear2Constraint, constraint,
                                         m_gear2Constraint);
    if (m_gear2Body1SignalConnection)
        QObject::disconnect(m_gear2Body1SignalConnection);
    if (m_gear2Body2SignalConnection)
        QObject::disconnect(m_gear2Body2SignalConnection);
    m_gear2Body1SignalConnection = {};
    m_gear2Body2SignalConnection = {};

    m_gear2Constraint = constraint;
    watchGearHinge(m_gear2Constraint, m_gear2Body1SignalConnection, m_gear2Body2SignalConnection);
    linkGearConstraints();

    emit gear2ConstraintChanged(m_gear2Constraint);
}

float GearConstraint::getTotalLambda() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::GearConstraint *>(m_constraint)->GetTotalLambda();
}

void GearConstraint::watchGearHinge(HingeConstraint *hinge, QMetaObject::Connection &body1Connection,
                                    QMetaObject::Connection &body2Connection)
{
    if (!hinge)
        return;

    if (Body *b1 = hinge->body1()) {
        body1Connection = QObject::connect(b1, &Body::bodyIDChanged, this,
                                           [this] { linkGearConstraints(); });
    }
    if (Body *b2 = hinge->body2()) {
        body2Connection = QObject::connect(b2, &Body::bodyIDChanged, this,
                                           [this] { linkGearConstraints(); });
    }
}

void GearConstraint::linkGearConstraints()
{
    if (!m_constraint || !m_gear1Constraint || !m_gear2Constraint)
        return;

    if (!m_gear1Constraint->m_constraint || !m_gear2Constraint->m_constraint)
        return;

    static_cast<JPH::GearConstraint *>(m_constraint)->SetConstraints(m_gear1Constraint->m_constraint,
                                                                    m_gear2Constraint->m_constraint);
}

void GearConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mHingeAxis1 = PhysicsUtils::toJoltType(m_hingeAxis1);
    m_constraintSettings.mHingeAxis2 = PhysicsUtils::toJoltType(m_hingeAxis2);
    m_constraintSettings.mRatio = m_ratio;

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
    linkGearConstraints();
}
