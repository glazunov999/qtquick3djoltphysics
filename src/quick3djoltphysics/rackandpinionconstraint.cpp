#include "rackandpinionconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

#include <QtMath>

RackAndPinionConstraint::RackAndPinionConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
}

RackAndPinionConstraint::~RackAndPinionConstraint() = default;

QVector3D RackAndPinionConstraint::hingeAxis() const
{
    return m_hingeAxis;
}

void RackAndPinionConstraint::setHingeAxis(const QVector3D &hingeAxis)
{
    if (m_hingeAxis == hingeAxis)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'hingeAxis' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_hingeAxis = hingeAxis;
    emit hingeAxisChanged(m_hingeAxis);
}

QVector3D RackAndPinionConstraint::sliderAxis() const
{
    return m_sliderAxis;
}

void RackAndPinionConstraint::setSliderAxis(const QVector3D &sliderAxis)
{
    if (m_sliderAxis == sliderAxis)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'sliderAxis' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_sliderAxis = sliderAxis;
    emit sliderAxisChanged(m_sliderAxis);
}

float RackAndPinionConstraint::ratio() const
{
    return m_ratio;
}

void RackAndPinionConstraint::setRatio(float ratio)
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

HingeConstraint *RackAndPinionConstraint::pinionConstraint() const
{
    return m_pinionConstraint;
}

void RackAndPinionConstraint::setPinionConstraint(HingeConstraint *constraint)
{
    if (m_pinionConstraint == constraint)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &RackAndPinionConstraint::setPinionConstraint,
                                         constraint, m_pinionConstraint);
    if (m_pinionBody1SignalConnection)
        QObject::disconnect(m_pinionBody1SignalConnection);
    if (m_pinionBody2SignalConnection)
        QObject::disconnect(m_pinionBody2SignalConnection);
    m_pinionBody1SignalConnection = {};
    m_pinionBody2SignalConnection = {};

    m_pinionConstraint = constraint;
    watchPinionHinge(m_pinionConstraint, m_pinionBody1SignalConnection,
                     m_pinionBody2SignalConnection);
    linkRackAndPinionConstraints();

    emit pinionConstraintChanged(m_pinionConstraint);
}

SliderConstraint *RackAndPinionConstraint::rackConstraint() const
{
    return m_rackConstraint;
}

void RackAndPinionConstraint::setRackConstraint(SliderConstraint *constraint)
{
    if (m_rackConstraint == constraint)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &RackAndPinionConstraint::setRackConstraint,
                                         constraint, m_rackConstraint);
    if (m_rackBody1SignalConnection)
        QObject::disconnect(m_rackBody1SignalConnection);
    if (m_rackBody2SignalConnection)
        QObject::disconnect(m_rackBody2SignalConnection);
    m_rackBody1SignalConnection = {};
    m_rackBody2SignalConnection = {};

    m_rackConstraint = constraint;
    watchRackSlider(m_rackConstraint, m_rackBody1SignalConnection, m_rackBody2SignalConnection);
    linkRackAndPinionConstraints();

    emit rackConstraintChanged(m_rackConstraint);
}

float RackAndPinionConstraint::getTotalLambda() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::RackAndPinionConstraint *>(m_constraint)->GetTotalLambda();
}

float RackAndPinionConstraint::ratioFromTeeth(int rackNumTeeth, float rackLength, int pinionNumTeeth)
{
    if (pinionNumTeeth <= 0 || rackLength <= 0.0f)
        return 1.0f;
    return float(2.0 * M_PI * rackNumTeeth / (rackLength * pinionNumTeeth));
}

void RackAndPinionConstraint::watchPinionHinge(HingeConstraint *hinge,
                                             QMetaObject::Connection &body1Connection,
                                             QMetaObject::Connection &body2Connection)
{
    if (!hinge)
        return;

    if (Body *b1 = hinge->body1()) {
        body1Connection = QObject::connect(b1, &Body::bodyIDChanged, this,
                                           [this] { linkRackAndPinionConstraints(); });
    }
    if (Body *b2 = hinge->body2()) {
        body2Connection = QObject::connect(b2, &Body::bodyIDChanged, this,
                                           [this] { linkRackAndPinionConstraints(); });
    }
}

void RackAndPinionConstraint::watchRackSlider(SliderConstraint *slider,
                                              QMetaObject::Connection &body1Connection,
                                              QMetaObject::Connection &body2Connection)
{
    if (!slider)
        return;

    if (Body *b1 = slider->body1()) {
        body1Connection = QObject::connect(b1, &Body::bodyIDChanged, this,
                                           [this] { linkRackAndPinionConstraints(); });
    }
    if (Body *b2 = slider->body2()) {
        body2Connection = QObject::connect(b2, &Body::bodyIDChanged, this,
                                           [this] { linkRackAndPinionConstraints(); });
    }
}

void RackAndPinionConstraint::linkRackAndPinionConstraints()
{
    if (!m_constraint || !m_pinionConstraint || !m_rackConstraint)
        return;

    if (!m_pinionConstraint->m_constraint || !m_rackConstraint->m_constraint)
        return;

    static_cast<JPH::RackAndPinionConstraint *>(m_constraint)->SetConstraints(
        m_pinionConstraint->m_constraint, m_rackConstraint->m_constraint);
}

void RackAndPinionConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mHingeAxis = PhysicsUtils::toJoltType(m_hingeAxis);
    m_constraintSettings.mSliderAxis = PhysicsUtils::toJoltType(m_sliderAxis);
    m_constraintSettings.mRatio = m_ratio;

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
    linkRackAndPinionConstraints();
}
