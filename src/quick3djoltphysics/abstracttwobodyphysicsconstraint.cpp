#include "abstracttwobodyphysicsconstraint_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQuick3D/private/qquick3dobject_p.h>

AbstractTwoBodyPhysicsConstraintSettings::AbstractTwoBodyPhysicsConstraintSettings(QObject *parent)
    : AbstractPhysicsConstraintSettings(parent)
{
}

AbstractTwoBodyPhysicsConstraintSettings::~AbstractTwoBodyPhysicsConstraintSettings() = default;

AbstractTwoBodyPhysicsConstraintSettings::ConstraintSpace AbstractTwoBodyPhysicsConstraintSettings::space() const
{
    return m_space;
}

void AbstractTwoBodyPhysicsConstraintSettings::setSpace(ConstraintSpace space)
{
    if (m_space == space)
        return;

    m_space = space;
    emit spaceChanged(m_space);
    emit changed();
}

JPH::Ref<JPH::ConstraintSettings> AbstractTwoBodyPhysicsConstraintSettings::createJoltConstraintSettings() const
{
    return static_cast<JPH::ConstraintSettings *>(createJoltTwoBodyConstraintSettings().GetPtr());
}

void AbstractTwoBodyPhysicsConstraintSettings::mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                                                          const QQuick3DNode *localFrame) const
{
    Q_UNUSED(settings);
    Q_UNUSED(localFrame);
}

bool AbstractTwoBodyPhysicsConstraintSettings::canMapToWorld(const QQuick3DNode *localFrame) const
{
    return localFrame != nullptr && m_space == ConstraintSpace::WorldSpace;
}

void AbstractTwoBodyPhysicsConstraintSettings::mapPositionToWorld(JPH::Vec3 &position,
                                                                  const QQuick3DNode *localFrame)
{
    position = PhysicsUtils::toJoltType(
            localFrame->mapPositionToScene(PhysicsUtils::toQtType(position)));
}

void AbstractTwoBodyPhysicsConstraintSettings::mapDirectionToWorld(JPH::Vec3 &direction,
                                                                   const QQuaternion &rotation)
{
    direction = PhysicsUtils::toJoltType(
            rotation.rotatedVector(PhysicsUtils::toQtType(direction)).normalized());
}

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

