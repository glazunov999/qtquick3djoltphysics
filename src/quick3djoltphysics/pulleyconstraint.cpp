#include "pulleyconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

PulleyConstraintSettings::PulleyConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D PulleyConstraintSettings::bodyPoint1() const
{
    return m_bodyPoint1;
}

void PulleyConstraintSettings::setBodyPoint1(const QVector3D &bodyPoint1)
{
    if (m_bodyPoint1 == bodyPoint1)
        return;

    m_bodyPoint1 = bodyPoint1;
    emit bodyPoint1Changed(m_bodyPoint1);
    emit changed();
}

QVector3D PulleyConstraintSettings::fixedPoint1() const
{
    return m_fixedPoint1;
}

void PulleyConstraintSettings::setFixedPoint1(const QVector3D &fixedPoint1)
{
    if (m_fixedPoint1 == fixedPoint1)
        return;

    m_fixedPoint1 = fixedPoint1;
    emit fixedPoint1Changed(m_fixedPoint1);
    emit changed();
}

QVector3D PulleyConstraintSettings::bodyPoint2() const
{
    return m_bodyPoint2;
}

void PulleyConstraintSettings::setBodyPoint2(const QVector3D &bodyPoint2)
{
    if (m_bodyPoint2 == bodyPoint2)
        return;

    m_bodyPoint2 = bodyPoint2;
    emit bodyPoint2Changed(m_bodyPoint2);
    emit changed();
}

QVector3D PulleyConstraintSettings::fixedPoint2() const
{
    return m_fixedPoint2;
}

void PulleyConstraintSettings::setFixedPoint2(const QVector3D &fixedPoint2)
{
    if (m_fixedPoint2 == fixedPoint2)
        return;

    m_fixedPoint2 = fixedPoint2;
    emit fixedPoint2Changed(m_fixedPoint2);
    emit changed();
}

float PulleyConstraintSettings::ratio() const
{
    return m_ratio;
}

void PulleyConstraintSettings::setRatio(float ratio)
{
    if (qFuzzyCompare(m_ratio, ratio))
        return;

    m_ratio = ratio;
    emit ratioChanged(m_ratio);
    emit changed();
}

float PulleyConstraintSettings::minLength() const
{
    return m_minLength;
}

void PulleyConstraintSettings::setMinLength(float minLength)
{
    if (qFuzzyCompare(m_minLength, minLength))
        return;

    m_minLength = minLength;
    emit minLengthChanged(m_minLength);
    emit changed();
}

float PulleyConstraintSettings::maxLength() const
{
    return m_maxLength;
}

void PulleyConstraintSettings::setMaxLength(float maxLength)
{
    if (qFuzzyCompare(m_maxLength, maxLength))
        return;

    m_maxLength = maxLength;
    emit maxLengthChanged(m_maxLength);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> PulleyConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::PulleyConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mBodyPoint1 = PhysicsUtils::toJoltType(m_bodyPoint1);
    settings->mFixedPoint1 = PhysicsUtils::toJoltType(m_fixedPoint1);
    settings->mBodyPoint2 = PhysicsUtils::toJoltType(m_bodyPoint2);
    settings->mFixedPoint2 = PhysicsUtils::toJoltType(m_fixedPoint2);
    settings->mRatio = m_ratio;
    settings->mMinLength = m_minLength;
    settings->mMaxLength = m_maxLength;
    mapToWorld(settings, localFrame);
    return settings;
}

void PulleyConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                          const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *pulley = JPH::DynamicCast<JPH::PulleyConstraintSettings>(settings);
    if (pulley == nullptr)
        return;

    mapPositionToWorld(pulley->mBodyPoint1, localFrame);
    mapPositionToWorld(pulley->mFixedPoint1, localFrame);
    mapPositionToWorld(pulley->mBodyPoint2, localFrame);
    mapPositionToWorld(pulley->mFixedPoint2, localFrame);
}

PulleyConstraint::PulleyConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new PulleyConstraintSettings(this));
}

PulleyConstraint::~PulleyConstraint() = default;

PulleyConstraintSettings *PulleyConstraint::settings() const
{
    return m_settings;
}

void PulleyConstraint::setSettings(PulleyConstraintSettings *settings)
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
    if (m_jolt == nullptr || !joltBodiesReady() || m_settings == nullptr)
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    const JPH::Ref<JPH::TwoBodyConstraintSettings> settings =
            m_settings->createJoltTwoBodyConstraintSettings();
    m_constraint = settings->Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
