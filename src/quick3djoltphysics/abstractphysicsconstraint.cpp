#include "abstractphysicsconstraint_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

AbstractPhysicsConstraintSettings::AbstractPhysicsConstraintSettings(QObject *parent)
    : QObject(parent)
{
}

AbstractPhysicsConstraintSettings::~AbstractPhysicsConstraintSettings() = default;

bool AbstractPhysicsConstraintSettings::enabled() const
{
    return m_enabled;
}

void AbstractPhysicsConstraintSettings::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
    emit changed();
}

quint32 AbstractPhysicsConstraintSettings::constraintPriority() const
{
    return m_constraintPriority;
}

void AbstractPhysicsConstraintSettings::setConstraintPriority(quint32 constraintPriority)
{
    if (m_constraintPriority == constraintPriority)
        return;

    m_constraintPriority = constraintPriority;
    emit constraintPriorityChanged(m_constraintPriority);
    emit changed();
}

uint AbstractPhysicsConstraintSettings::numVelocityStepsOverride() const
{
    return m_numVelocityStepsOverride;
}

void AbstractPhysicsConstraintSettings::setNumVelocityStepsOverride(uint numVelocityStepsOverride)
{
    if (m_numVelocityStepsOverride == numVelocityStepsOverride)
        return;

    m_numVelocityStepsOverride = numVelocityStepsOverride;
    emit numVelocityStepsOverrideChanged(m_numVelocityStepsOverride);
    emit changed();
}

uint AbstractPhysicsConstraintSettings::numPositionStepsOverride() const
{
    return m_numPositionStepsOverride;
}

void AbstractPhysicsConstraintSettings::setNumPositionStepsOverride(uint numPositionStepsOverride)
{
    if (m_numPositionStepsOverride == numPositionStepsOverride)
        return;

    m_numPositionStepsOverride = numPositionStepsOverride;
    emit numPositionStepsOverrideChanged(m_numPositionStepsOverride);
    emit changed();
}

float AbstractPhysicsConstraintSettings::drawConstraintSize() const
{
    return m_drawConstraintSize;
}

void AbstractPhysicsConstraintSettings::setDrawConstraintSize(float drawConstraintSize)
{
    if (qFuzzyCompare(m_drawConstraintSize, drawConstraintSize))
        return;

    m_drawConstraintSize = drawConstraintSize;
    emit drawConstraintSizeChanged(m_drawConstraintSize);
    emit changed();
}

void AbstractPhysicsConstraintSettings::applyBaseSettings(JPH::ConstraintSettings &settings) const
{
    settings.mEnabled = m_enabled;
    settings.mConstraintPriority = m_constraintPriority;
    settings.mNumVelocityStepsOverride = m_numVelocityStepsOverride;
    settings.mNumPositionStepsOverride = m_numPositionStepsOverride;
    settings.mDrawConstraintSize = m_drawConstraintSize;
}

AbstractPhysicsConstraint::AbstractPhysicsConstraint(QQuick3DNode *parent) : AbstractPhysicsNode(parent)
{
}

AbstractPhysicsConstraint::~AbstractPhysicsConstraint()
{
    cleanup();
}

void AbstractPhysicsConstraint::cleanup()
{
    if (m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraint = nullptr;
}
