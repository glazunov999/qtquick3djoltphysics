#ifndef MOTORSETTINGS_P_H
#define MOTORSETTINGS_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/MotorSettings.h>

#include <cfloat>

class Q_QUICK3DJOLTPHYSICS_EXPORT MotorSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SpringMode springMode READ springMode WRITE setSpringMode NOTIFY springModeChanged)
    Q_PROPERTY(float springFrequency READ springFrequency WRITE setSpringFrequency NOTIFY springFrequencyChanged)
    Q_PROPERTY(float springStiffness READ springStiffness WRITE setSpringStiffness NOTIFY springStiffnessChanged)
    Q_PROPERTY(float springDamping READ springDamping WRITE setSpringDamping NOTIFY springDampingChanged)
    Q_PROPERTY(float minForceLimit READ minForceLimit WRITE setMinForceLimit NOTIFY minForceLimitChanged)
    Q_PROPERTY(float maxForceLimit READ maxForceLimit WRITE setMaxForceLimit NOTIFY maxForceLimitChanged)
    Q_PROPERTY(float minTorqueLimit READ minTorqueLimit WRITE setMinTorqueLimit NOTIFY minTorqueLimitChanged)
    Q_PROPERTY(float maxTorqueLimit READ maxTorqueLimit WRITE setMaxTorqueLimit NOTIFY maxTorqueLimitChanged)
    QML_NAMED_ELEMENT(MotorSettings)
public:
    explicit MotorSettings(QObject *parent = nullptr);

    enum class SpringMode {
        FrequencyAndDamping,
        StiffnessAndDamping,
    };
    Q_ENUM(SpringMode)

    SpringMode springMode() const;
    void setSpringMode(SpringMode springMode);

    float springFrequency() const;
    void setSpringFrequency(float springFrequency);

    float springStiffness() const;
    void setSpringStiffness(float springStiffness);

    float springDamping() const;
    void setSpringDamping(float springDamping);

    float minForceLimit() const;
    void setMinForceLimit(float minForceLimit);

    float maxForceLimit() const;
    void setMaxForceLimit(float maxForceLimit);

    float minTorqueLimit() const;
    void setMinTorqueLimit(float minTorqueLimit);

    float maxTorqueLimit() const;
    void setMaxTorqueLimit(float maxTorqueLimit);

signals:
    void springModeChanged(SpringMode springMode);
    void springFrequencyChanged(float springFrequency);
    void springStiffnessChanged(float springStiffness);
    void springDampingChanged(float springDamping);
    void minForceLimitChanged(float minForceLimit);
    void maxForceLimitChanged(float maxForceLimit);
    void minTorqueLimitChanged(float minTorqueLimit);
    void maxTorqueLimitChanged(float maxTorqueLimit);
    void changed();

private:
    JPH::MotorSettings getJoltMotorSettings() const;
    friend class PathConstraint;

    SpringMode m_springMode = SpringMode::FrequencyAndDamping;
    float m_springFrequency = 2.0f;
    float m_springStiffness = 0.0f;
    float m_springDamping = 1.0f;
    float m_minForceLimit = -FLT_MAX;
    float m_maxForceLimit = FLT_MAX;
    float m_minTorqueLimit = -FLT_MAX;
    float m_maxTorqueLimit = FLT_MAX;
};

#endif // MOTORSETTINGS_P_H
