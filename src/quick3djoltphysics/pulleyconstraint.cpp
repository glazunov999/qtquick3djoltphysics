#include "pulleyconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PulleyConstraint::PulleyConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

PulleyConstraint::~PulleyConstraint() = default;

QVector3D PulleyConstraint::bodyPoint1() const
{
    return m_bodyPoint1;
}

void PulleyConstraint::setBodyPoint1(const QVector3D &point)
{
    if (m_bodyPoint1 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'bodyPoint1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_bodyPoint1 = point;
    emit bodyPoint1Changed(m_bodyPoint1);
}

QVector3D PulleyConstraint::fixedPoint1() const
{
    return m_fixedPoint1;
}

void PulleyConstraint::setFixedPoint1(const QVector3D &point)
{
    if (m_fixedPoint1 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'fixedPoint1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_fixedPoint1 = point;
    emit fixedPoint1Changed(m_fixedPoint1);
}

QVector3D PulleyConstraint::bodyPoint2() const
{
    return m_bodyPoint2;
}

void PulleyConstraint::setBodyPoint2(const QVector3D &point)
{
    if (m_bodyPoint2 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'bodyPoint2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_bodyPoint2 = point;
    emit bodyPoint2Changed(m_bodyPoint2);
}

QVector3D PulleyConstraint::fixedPoint2() const
{
    return m_fixedPoint2;
}

void PulleyConstraint::setFixedPoint2(const QVector3D &point)
{
    if (m_fixedPoint2 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'fixedPoint2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_fixedPoint2 = point;
    emit fixedPoint2Changed(m_fixedPoint2);
}

float PulleyConstraint::ratio() const
{
    return m_ratio;
}

void PulleyConstraint::setRatio(float ratio)
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

float PulleyConstraint::minLength() const
{
    return m_minLength;
}

void PulleyConstraint::setMinLength(float minLength)
{
    if (qFuzzyCompare(m_minLength, minLength))
        return;

    m_minLength = minLength;

    if (m_constraint) {
        auto *pulley = static_cast<JPH::PulleyConstraint *>(m_constraint);
        const float maxL = m_maxLength < 0.0f ? pulley->GetMaxLength() : m_maxLength;
        if (m_minLength >= 0.0f && m_minLength <= maxL)
            pulley->SetLength(m_minLength, maxL);
    }

    emit minLengthChanged(m_minLength);
}

float PulleyConstraint::maxLength() const
{
    return m_maxLength;
}

void PulleyConstraint::setMaxLength(float maxLength)
{
    if (qFuzzyCompare(m_maxLength, maxLength))
        return;

    m_maxLength = maxLength;

    if (m_constraint) {
        auto *pulley = static_cast<JPH::PulleyConstraint *>(m_constraint);
        const float minL = m_minLength < 0.0f ? pulley->GetMinLength() : m_minLength;
        const float maxL = m_maxLength < 0.0f ? pulley->GetMaxLength() : m_maxLength;
        if (minL >= 0.0f && minL <= maxL)
            pulley->SetLength(minL, maxL);
    }

    emit maxLengthChanged(m_maxLength);
}

float PulleyConstraint::getCurrentLength() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::PulleyConstraint *>(m_constraint)->GetCurrentLength();
}

float PulleyConstraint::getTotalLambdaPosition() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::PulleyConstraint *>(m_constraint)->GetTotalLambdaPosition();
}

void PulleyConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mBodyPoint1 = PhysicsUtils::toJoltType(m_bodyPoint1);
    m_constraintSettings.mFixedPoint1 = PhysicsUtils::toJoltType(m_fixedPoint1);
    m_constraintSettings.mBodyPoint2 = PhysicsUtils::toJoltType(m_bodyPoint2);
    m_constraintSettings.mFixedPoint2 = PhysicsUtils::toJoltType(m_fixedPoint2);
    m_constraintSettings.mRatio = m_ratio;
    m_constraintSettings.mMinLength = m_minLength;
    m_constraintSettings.mMaxLength = m_maxLength < 0.0f ? -1.0f : m_maxLength;

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
