#include "sliderconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

namespace {

bool sanitizeSliderLimits(float &limitsMin, float &limitsMax)
{
    bool changed = false;
    if (limitsMin > 0.0f) {
        qWarning() << "SliderConstraint: limitsMin must be <= 0 (Jolt: position 0 at anchor); clamping"
                   << limitsMin << "to 0";
        limitsMin = 0.0f;
        changed = true;
    }
    if (limitsMax < 0.0f) {
        qWarning() << "SliderConstraint: limitsMax must be >= 0; clamping" << limitsMax << "to 0";
        limitsMax = 0.0f;
        changed = true;
    }
    if (limitsMin > limitsMax) {
        qWarning() << "SliderConstraint: limitsMin must be <= limitsMax; swapping" << limitsMin
                   << "and" << limitsMax;
        const float tmp = limitsMin;
        limitsMin = limitsMax;
        limitsMax = tmp;
        changed = true;
    }
    return changed;
}

} // namespace

SliderConstraint::SliderConstraint(QQuick3DNode *parent) : AbstractTwoBodyPhysicsConstraint(parent)
{
}

SliderConstraint::~SliderConstraint() = default;

QVector3D SliderConstraint::point1() const
{
    return m_point1;
}

void SliderConstraint::setPoint1(const QVector3D &point)
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

QVector3D SliderConstraint::point2() const
{
    return m_point2;
}

void SliderConstraint::setPoint2(const QVector3D &point)
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

bool SliderConstraint::autoDetectPoint() const
{
    return m_autoDetectPoint;
}

void SliderConstraint::setAutoDetectPoint(bool autoDetectPoint)
{
    if (m_autoDetectPoint == autoDetectPoint)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'autoDetectPoint' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_autoDetectPoint = autoDetectPoint;
    emit autoDetectPointChanged(m_autoDetectPoint);
}

QVector3D SliderConstraint::sliderAxis1() const
{
    return m_sliderAxis1;
}

void SliderConstraint::setSliderAxis1(const QVector3D &sliderAxis1)
{
    if (m_sliderAxis1 == sliderAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'sliderAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_sliderAxis1 = sliderAxis1;
    emit sliderAxis1Changed(m_sliderAxis1);
}

QVector3D SliderConstraint::sliderAxis2() const
{
    return m_sliderAxis2;
}

void SliderConstraint::setSliderAxis2(const QVector3D &sliderAxis2)
{
    if (m_sliderAxis2 == sliderAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'sliderAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_sliderAxis2 = sliderAxis2;
    emit sliderAxis2Changed(m_sliderAxis2);
}

QVector3D SliderConstraint::normalAxis1() const
{
    return m_normalAxis1;
}

void SliderConstraint::setNormalAxis1(const QVector3D &normalAxis1)
{
    if (m_normalAxis1 == normalAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'normalAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_normalAxis1 = normalAxis1;
    emit normalAxis1Changed(m_normalAxis1);
}

QVector3D SliderConstraint::normalAxis2() const
{
    return m_normalAxis2;
}

void SliderConstraint::setNormalAxis2(const QVector3D &normalAxis2)
{
    if (m_normalAxis2 == normalAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'normalAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_normalAxis2 = normalAxis2;
    emit normalAxis2Changed(m_normalAxis2);
}

float SliderConstraint::limitsMin() const
{
    return m_limitsMin;
}

void SliderConstraint::setLimitsMin(float limitsMin)
{
    const float oldMin = m_limitsMin;
    const float oldMax = m_limitsMax;
    float min = limitsMin;
    float max = m_limitsMax;
    sanitizeSliderLimits(min, max);
    if (qFuzzyCompare(oldMin, min) && qFuzzyCompare(oldMax, max))
        return;

    m_limitsMin = min;
    m_limitsMax = max;
    if (m_constraint)
        static_cast<JPH::SliderConstraint *>(m_constraint)->SetLimits(m_limitsMin, m_limitsMax);

    if (!qFuzzyCompare(oldMin, min))
        emit limitsMinChanged(m_limitsMin);
    if (!qFuzzyCompare(oldMax, max))
        emit limitsMaxChanged(m_limitsMax);
}

float SliderConstraint::limitsMax() const
{
    return m_limitsMax;
}

void SliderConstraint::setLimitsMax(float limitsMax)
{
    const float oldMin = m_limitsMin;
    const float oldMax = m_limitsMax;
    float min = m_limitsMin;
    float max = limitsMax;
    sanitizeSliderLimits(min, max);
    if (qFuzzyCompare(oldMin, min) && qFuzzyCompare(oldMax, max))
        return;

    m_limitsMin = min;
    m_limitsMax = max;
    if (m_constraint)
        static_cast<JPH::SliderConstraint *>(m_constraint)->SetLimits(m_limitsMin, m_limitsMax);

    if (!qFuzzyCompare(oldMin, min))
        emit limitsMinChanged(m_limitsMin);
    if (!qFuzzyCompare(oldMax, max))
        emit limitsMaxChanged(m_limitsMax);
}

SpringSettings *SliderConstraint::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void SliderConstraint::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
{
    if (m_limitsSpringSettings == limitsSpringSettings)
        return;

    if (m_limitsSpringSettingsConnection)
        QObject::disconnect(m_limitsSpringSettingsConnection);
    m_limitsSpringSettingsConnection = {};

    m_limitsSpringSettings = limitsSpringSettings;

    if (m_limitsSpringSettings) {
        m_limitsSpringSettingsConnection = QObject::connect(
            m_limitsSpringSettings, &SpringSettings::changed, this, [this] {
                if (m_constraint && m_limitsSpringSettings)
                    static_cast<JPH::SliderConstraint *>(m_constraint)->SetLimitsSpringSettings(
                        m_limitsSpringSettings->getJoltSpringSettings());
            });
        QObject::connect(m_limitsSpringSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_limitsSpringSettings == obj)
                setLimitsSpringSettings(nullptr);
        });
    }

    if (m_constraint) {
        if (m_limitsSpringSettings)
            static_cast<JPH::SliderConstraint *>(m_constraint)->SetLimitsSpringSettings(
                m_limitsSpringSettings->getJoltSpringSettings());
        else
            static_cast<JPH::SliderConstraint *>(m_constraint)->SetLimitsSpringSettings(
                JPH::SpringSettings());
    }

    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
}

float SliderConstraint::maxFrictionForce() const
{
    return m_maxFrictionForce;
}

void SliderConstraint::setMaxFrictionForce(float maxFrictionForce)
{
    if (qFuzzyCompare(m_maxFrictionForce, maxFrictionForce))
        return;

    m_maxFrictionForce = maxFrictionForce;

    if (m_constraint)
        static_cast<JPH::SliderConstraint *>(m_constraint)->SetMaxFrictionForce(m_maxFrictionForce);

    emit maxFrictionForceChanged(m_maxFrictionForce);
}

float SliderConstraint::getCurrentPosition() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::SliderConstraint *>(m_constraint)->GetCurrentPosition();
}

void SliderConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || !joltBodiesReady())
        return;

    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraintSettings.mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    m_constraintSettings.mAutoDetectPoint = m_autoDetectPoint;
    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraintSettings.mSliderAxis1 = PhysicsUtils::toJoltType(m_sliderAxis1);
    m_constraintSettings.mSliderAxis2 = PhysicsUtils::toJoltType(m_sliderAxis2);
    m_constraintSettings.mNormalAxis1 = PhysicsUtils::toJoltType(m_normalAxis1);
    m_constraintSettings.mNormalAxis2 = PhysicsUtils::toJoltType(m_normalAxis2);
    sanitizeSliderLimits(m_limitsMin, m_limitsMax);
    m_constraintSettings.mLimitsMin = m_limitsMin;
    m_constraintSettings.mLimitsMax = m_limitsMax;
    m_constraintSettings.mMaxFrictionForce = m_maxFrictionForce;

    if (m_limitsSpringSettings)
        m_constraintSettings.mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();

    m_constraint = m_constraintSettings.Create(*joltBody1(), *joltBody2());
    m_jolt->AddConstraint(m_constraint);
}
