#include "motorsettings_p.h"

MotorSettings::MotorSettings(QObject *parent) : QObject(parent)
{
}

MotorSettings::SpringMode MotorSettings::springMode() const
{
    return m_springMode;
}

void MotorSettings::setSpringMode(SpringMode springMode)
{
    if (m_springMode == springMode)
        return;

    m_springMode = springMode;
    emit springModeChanged(m_springMode);
    emit changed();
}

float MotorSettings::springFrequency() const
{
    return m_springFrequency;
}

void MotorSettings::setSpringFrequency(float springFrequency)
{
    if (qFuzzyCompare(m_springFrequency, springFrequency))
        return;

    m_springFrequency = springFrequency;
    emit springFrequencyChanged(m_springFrequency);
    emit changed();
}

float MotorSettings::springStiffness() const
{
    return m_springStiffness;
}

void MotorSettings::setSpringStiffness(float springStiffness)
{
    if (qFuzzyCompare(m_springStiffness, springStiffness))
        return;

    m_springStiffness = springStiffness;
    emit springStiffnessChanged(m_springStiffness);
    emit changed();
}

float MotorSettings::springDamping() const
{
    return m_springDamping;
}

void MotorSettings::setSpringDamping(float springDamping)
{
    if (qFuzzyCompare(m_springDamping, springDamping))
        return;

    m_springDamping = springDamping;
    emit springDampingChanged(m_springDamping);
    emit changed();
}

float MotorSettings::minForceLimit() const
{
    return m_minForceLimit;
}

void MotorSettings::setMinForceLimit(float minForceLimit)
{
    if (qFuzzyCompare(m_minForceLimit, minForceLimit))
        return;

    m_minForceLimit = minForceLimit;
    emit minForceLimitChanged(m_minForceLimit);
    emit changed();
}

float MotorSettings::maxForceLimit() const
{
    return m_maxForceLimit;
}

void MotorSettings::setMaxForceLimit(float maxForceLimit)
{
    if (qFuzzyCompare(m_maxForceLimit, maxForceLimit))
        return;

    m_maxForceLimit = maxForceLimit;
    emit maxForceLimitChanged(m_maxForceLimit);
    emit changed();
}

float MotorSettings::minTorqueLimit() const
{
    return m_minTorqueLimit;
}

void MotorSettings::setMinTorqueLimit(float minTorqueLimit)
{
    if (qFuzzyCompare(m_minTorqueLimit, minTorqueLimit))
        return;

    m_minTorqueLimit = minTorqueLimit;
    emit minTorqueLimitChanged(m_minTorqueLimit);
    emit changed();
}

float MotorSettings::maxTorqueLimit() const
{
    return m_maxTorqueLimit;
}

void MotorSettings::setMaxTorqueLimit(float maxTorqueLimit)
{
    if (qFuzzyCompare(m_maxTorqueLimit, maxTorqueLimit))
        return;

    m_maxTorqueLimit = maxTorqueLimit;
    emit maxTorqueLimitChanged(m_maxTorqueLimit);
    emit changed();
}

JPH::MotorSettings MotorSettings::getJoltMotorSettings() const
{
    JPH::MotorSettings ms;
    switch (m_springMode) {
    case SpringMode::FrequencyAndDamping:
        ms.mSpringSettings.mMode = JPH::ESpringMode::FrequencyAndDamping;
        ms.mSpringSettings.mFrequency = m_springFrequency;
        break;
    case SpringMode::StiffnessAndDamping:
        ms.mSpringSettings.mMode = JPH::ESpringMode::StiffnessAndDamping;
        ms.mSpringSettings.mStiffness = m_springStiffness;
        break;
    }
    ms.mSpringSettings.mDamping = m_springDamping;
    ms.mMinForceLimit = m_minForceLimit;
    ms.mMaxForceLimit = m_maxForceLimit;
    ms.mMinTorqueLimit = m_minTorqueLimit;
    ms.mMaxTorqueLimit = m_maxTorqueLimit;
    return ms;
}
