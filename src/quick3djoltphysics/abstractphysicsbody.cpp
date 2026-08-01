#include "abstractphysicsbody_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/PhysicsSystem.h>

AbstractPhysicsBody::AbstractPhysicsBody(QQuick3DNode *parent) : AbstractPhysicsNode(parent)
{
}

AbstractPhysicsBody::~AbstractPhysicsBody()
{
    if (m_shape != nullptr)
        m_shape->disconnect(m_shapeSignalConnection);
}

quint32 AbstractPhysicsBody::bodyID() const
{
    return m_bodyID;
}

AbstractShape *AbstractPhysicsBody::shape() const
{
    return m_shape;
}

void AbstractPhysicsBody::setShape(AbstractShape *shape)
{
    if (m_shape == shape)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &AbstractPhysicsBody::setShape, shape, m_shape);
    if (m_shape != nullptr)
        m_shape->disconnect(m_shapeSignalConnection);
    m_shape = shape;
    if (m_shape) {
        if (m_shape->parentItem() == nullptr)
            m_shape->setParentItem(this);
        m_shapeSignalConnection = QObject::connect(m_shape, &AbstractShape::changed, this,
                                                   [this] { markJoltObjectDirty(); });
    }

    markJoltObjectDirty();
    emit shapeChanged(m_shape);
}
