#include "abstracttwobodyphysicsconstraint_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

AbstractTwoBodyPhysicsConstraint::AbstractTwoBodyPhysicsConstraint(QQuick3DNode *parent)
    : AbstractPhysicsConstraint(parent)
{
}

AbstractTwoBodyPhysicsConstraint::~AbstractTwoBodyPhysicsConstraint() = default;

Body *AbstractTwoBodyPhysicsConstraint::body1() const
{
    return m_body1;
}

void AbstractTwoBodyPhysicsConstraint::setBody1(Body *body)
{
    if (m_body1 == body)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &AbstractTwoBodyPhysicsConstraint::setBody1, body,
                                         m_body1);
    if (m_body1 != nullptr)
        m_body1->disconnect(m_body1SignalConnection);
    m_body1 = body;
    if (m_body1) {
        m_body1SignalConnection = QObject::connect(m_body1, &Body::bodyIDChanged, this,
                                                   [this] { updateJoltObject(); });
    }

    updateJoltObject();
    emit body1Changed(m_body1);
}

Body *AbstractTwoBodyPhysicsConstraint::body2() const
{
    return m_body2;
}

void AbstractTwoBodyPhysicsConstraint::setBody2(Body *body)
{
    if (m_body2 == body)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &AbstractTwoBodyPhysicsConstraint::setBody2, body,
                                         m_body2);
    if (m_body2 != nullptr)
        m_body2->disconnect(m_body2SignalConnection);
    m_body2 = body;
    if (m_body2) {
        m_body2SignalConnection = QObject::connect(m_body2, &Body::bodyIDChanged, this,
                                                   [this] { updateJoltObject(); });
    }

    updateJoltObject();
    emit body2Changed(m_body2);
}

AbstractTwoBodyPhysicsConstraint::ConstraintSpace AbstractTwoBodyPhysicsConstraint::space() const
{
    return m_space;
}

void AbstractTwoBodyPhysicsConstraint::setSpace(ConstraintSpace space)
{
    if (m_space == space)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'space' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_space = space;
    emit spaceChanged(m_space);
}

bool AbstractTwoBodyPhysicsConstraint::joltBodiesReady() const
{
    return m_body1 != nullptr && m_body2 != nullptr && m_body1->m_body != nullptr
            && m_body2->m_body != nullptr;
}

JPH::Body *AbstractTwoBodyPhysicsConstraint::joltBody1() const
{
    return m_body1 ? m_body1->m_body : nullptr;
}

JPH::Body *AbstractTwoBodyPhysicsConstraint::joltBody2() const
{
    return m_body2 ? m_body2->m_body : nullptr;
}
