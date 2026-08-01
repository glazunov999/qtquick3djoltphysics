#include "sliderconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>

SliderConstraintSettings::SliderConstraintSettings(QObject *parent)
    : AbstractTwoBodyPhysicsConstraintSettings(parent)
{
}

QVector3D SliderConstraintSettings::point1() const
{
    return m_point1;
}

void SliderConstraintSettings::setPoint1(const QVector3D &point1)
{
    if (m_point1 == point1)
        return;

    m_point1 = point1;
    emit point1Changed(m_point1);
    emit changed();
}

QVector3D SliderConstraintSettings::point2() const
{
    return m_point2;
}

void SliderConstraintSettings::setPoint2(const QVector3D &point2)
{
    if (m_point2 == point2)
        return;

    m_point2 = point2;
    emit point2Changed(m_point2);
    emit changed();
}

bool SliderConstraintSettings::autoDetectPoint() const
{
    return m_autoDetectPoint;
}

void SliderConstraintSettings::setAutoDetectPoint(bool autoDetectPoint)
{
    if (m_autoDetectPoint == autoDetectPoint)
        return;

    m_autoDetectPoint = autoDetectPoint;
    emit autoDetectPointChanged(m_autoDetectPoint);
    emit changed();
}

QVector3D SliderConstraintSettings::sliderAxis1() const
{
    return m_sliderAxis1;
}

void SliderConstraintSettings::setSliderAxis1(const QVector3D &sliderAxis1)
{
    if (m_sliderAxis1 == sliderAxis1)
        return;

    m_sliderAxis1 = sliderAxis1;
    emit sliderAxis1Changed(m_sliderAxis1);
    emit changed();
}

QVector3D SliderConstraintSettings::sliderAxis2() const
{
    return m_sliderAxis2;
}

void SliderConstraintSettings::setSliderAxis2(const QVector3D &sliderAxis2)
{
    if (m_sliderAxis2 == sliderAxis2)
        return;

    m_sliderAxis2 = sliderAxis2;
    emit sliderAxis2Changed(m_sliderAxis2);
    emit changed();
}

QVector3D SliderConstraintSettings::normalAxis1() const
{
    return m_normalAxis1;
}

void SliderConstraintSettings::setNormalAxis1(const QVector3D &normalAxis1)
{
    if (m_normalAxis1 == normalAxis1)
        return;

    m_normalAxis1 = normalAxis1;
    emit normalAxis1Changed(m_normalAxis1);
    emit changed();
}

QVector3D SliderConstraintSettings::normalAxis2() const
{
    return m_normalAxis2;
}

void SliderConstraintSettings::setNormalAxis2(const QVector3D &normalAxis2)
{
    if (m_normalAxis2 == normalAxis2)
        return;

    m_normalAxis2 = normalAxis2;
    emit normalAxis2Changed(m_normalAxis2);
    emit changed();
}

float SliderConstraintSettings::limitsMin() const
{
    return m_limitsMin;
}

void SliderConstraintSettings::setLimitsMin(float limitsMin)
{
    if (qFuzzyCompare(m_limitsMin, limitsMin))
        return;

    m_limitsMin = limitsMin;
    emit limitsMinChanged(m_limitsMin);
    emit changed();
}

float SliderConstraintSettings::limitsMax() const
{
    return m_limitsMax;
}

void SliderConstraintSettings::setLimitsMax(float limitsMax)
{
    if (qFuzzyCompare(m_limitsMax, limitsMax))
        return;

    m_limitsMax = limitsMax;
    emit limitsMaxChanged(m_limitsMax);
    emit changed();
}

SpringSettings *SliderConstraintSettings::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void SliderConstraintSettings::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
{
    if (m_limitsSpringSettings == limitsSpringSettings)
        return;

    if (m_limitsSpringSettings)
        m_limitsSpringSettings->disconnect(this);

    m_limitsSpringSettings = limitsSpringSettings;

    if (m_limitsSpringSettings) {
        connect(m_limitsSpringSettings, &SpringSettings::changed, this, &AbstractPhysicsConstraintSettings::changed);
        connect(m_limitsSpringSettings, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_limitsSpringSettings == obj)
                setLimitsSpringSettings(nullptr);
        });
    }

    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
    emit changed();
}

float SliderConstraintSettings::maxFrictionForce() const
{
    return m_maxFrictionForce;
}

void SliderConstraintSettings::setMaxFrictionForce(float maxFrictionForce)
{
    if (qFuzzyCompare(m_maxFrictionForce, maxFrictionForce))
        return;

    m_maxFrictionForce = maxFrictionForce;
    emit maxFrictionForceChanged(m_maxFrictionForce);
    emit changed();
}

JPH::Ref<JPH::TwoBodyConstraintSettings> SliderConstraintSettings::createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame) const
{
    auto *settings = new JPH::SliderConstraintSettings;
    applyBaseSettings(*settings);
    settings->mSpace = static_cast<JPH::EConstraintSpace>(m_space);
    settings->mPoint1 = PhysicsUtils::toJoltType(m_point1);
    settings->mPoint2 = PhysicsUtils::toJoltType(m_point2);
    settings->mAutoDetectPoint = m_autoDetectPoint;
    settings->mSliderAxis1 = PhysicsUtils::toJoltType(m_sliderAxis1);
    settings->mSliderAxis2 = PhysicsUtils::toJoltType(m_sliderAxis2);
    settings->mNormalAxis1 = PhysicsUtils::toJoltType(m_normalAxis1);
    settings->mNormalAxis2 = PhysicsUtils::toJoltType(m_normalAxis2);
    settings->mLimitsMin = m_limitsMin;
    settings->mLimitsMax = m_limitsMax;
    if (m_limitsSpringSettings)
        settings->mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();
    settings->mMaxFrictionForce = m_maxFrictionForce;
    mapToWorld(settings, localFrame);
    return settings;
}

void SliderConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                          const QQuick3DNode *localFrame) const
{
    if (settings == nullptr || !canMapToWorld(localFrame))
        return;

    auto *slider = JPH::DynamicCast<JPH::SliderConstraintSettings>(settings);
    if (slider == nullptr)
        return;

    const QQuaternion rotation = localFrame->sceneRotation();
    mapPositionToWorld(slider->mPoint1, localFrame);
    mapPositionToWorld(slider->mPoint2, localFrame);
    mapDirectionToWorld(slider->mSliderAxis1, rotation);
    mapDirectionToWorld(slider->mSliderAxis2, rotation);
    mapDirectionToWorld(slider->mNormalAxis1, rotation);
    mapDirectionToWorld(slider->mNormalAxis2, rotation);
}

SliderConstraint::SliderConstraint(QQuick3DNode *parent)
    : AbstractTwoBodyPhysicsConstraint(parent)
{
    setSettings(new SliderConstraintSettings(this));
}

SliderConstraint::~SliderConstraint() = default;

SliderConstraintSettings *SliderConstraint::settings() const
{
    return m_settings;
}

void SliderConstraint::setSettings(SliderConstraintSettings *settings)
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

float SliderConstraint::getCurrentPosition() const
{
    if (!m_constraint)
        return 0.0f;
    return static_cast<const JPH::SliderConstraint *>(m_constraint)->GetCurrentPosition();
}

void SliderConstraint::updateJoltObject()
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
