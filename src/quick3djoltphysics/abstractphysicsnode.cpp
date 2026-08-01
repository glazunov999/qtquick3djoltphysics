#include "abstractphysicsnode_p.h"
#include "physicssystem_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

AbstractPhysicsNode::AbstractPhysicsNode(QQuick3DNode *parent) : QQuick3DNode(parent)
{   
}

AbstractPhysicsNode::~AbstractPhysicsNode()
{
    PhysicsSystem::deregisterPhysicsNode(this);
}

AbstractPhysicsNode::Activation AbstractPhysicsNode::activation() const
{
    return m_activation;
}

void AbstractPhysicsNode::setActivation(Activation activation)
{
    if (m_activation == activation)
        return;

    m_activation = activation;
    emit activationChanged(activation);
}

void AbstractPhysicsNode::componentComplete()
{
    QQuick3DNode::componentComplete();
    PhysicsSystem::registerPhysicsNode(this);
}

void AbstractPhysicsNode::cleanup() { }

void AbstractPhysicsNode::preSync(float /*deltaTime*/, QHash<QQuick3DNode *, QMatrix4x4> & /*transformCache*/) { }

void AbstractPhysicsNode::sync() { }

void AbstractPhysicsNode::markJoltObjectDirty()
{
    if (m_removed)
        return;

    m_joltObjectDirty = true;
}

void AbstractPhysicsNode::rebuildJoltObjectIfDirty()
{
    if (!m_joltObjectDirty || m_removed || m_jolt == nullptr)
        return;

    m_joltObjectDirty = false;
    updateJoltObject();
}

void AbstractPhysicsNode::init(JPH::PhysicsSystem *jolt, JPH::TempAllocator *tempAllocator)
{
    Q_ASSERT(jolt);
    Q_ASSERT(tempAllocator);

    m_jolt = jolt;
    m_tempAllocator = tempAllocator;
    m_bodyInterface = &jolt->GetBodyInterface();
    m_joltObjectDirty = false;

    updateJoltObject();
}
