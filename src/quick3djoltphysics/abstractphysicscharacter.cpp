#include "abstractphysicscharacter_p.h"
#include "physicsutils_p.h"
#include "physicssystem_p.h"

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyLock.h>

AbstractPhysicsCharacter::AbstractPhysicsCharacter(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
    connect(this, &QQuick3DNode::upChanged, this, &AbstractPhysicsCharacter::handleUpChanged);
}

AbstractPhysicsCharacter::~AbstractPhysicsCharacter() = default;

QVector4D AbstractPhysicsCharacter::supportingVolume() const
{
    return m_supportingVolume;
}

void AbstractPhysicsCharacter::setSupportingVolume(const QVector4D &supportingVolume)
{
    if (m_supportingVolume == supportingVolume)
        return;

    if (character()) {
        qWarning() << "Warning: Changing 'supportingVolume' after character is initialized will have "
                      "no effect";
        return;
    }

    m_supportingVolume = supportingVolume;
    emit supportingVolumeChanged(supportingVolume);
}

float AbstractPhysicsCharacter::maxSlopeAngle() const
{
    return m_maxSlopeAngle;
}

void AbstractPhysicsCharacter::setMaxSlopeAngle(float maxSlopeAngle)
{
    if (qFuzzyCompare(m_maxSlopeAngle, maxSlopeAngle))
        return;

    if (character()) {
        qWarning() << "Warning: Changing 'maxSlopeAngle' after character is initialized will have "
                      "no effect";
        return;
    }

    m_maxSlopeAngle = maxSlopeAngle;
    emit maxSlopeAngleChanged(maxSlopeAngle);
}

bool AbstractPhysicsCharacter::isSlopeTooSteep(const QVector3D &normal) const
{
    if (character() == nullptr)
        return false;

    return character()->IsSlopeTooSteep(PhysicsUtils::toJoltType(normal));
}

bool AbstractPhysicsCharacter::isSupported() const
{
    if (character() == nullptr)
        return false;

    return character()->IsSupported();
}

AbstractPhysicsCharacter::GroundState AbstractPhysicsCharacter::getGroundState() const
{
    if (character() == nullptr)
        return GroundState::OnGround;

    return static_cast<GroundState>(character()->GetGroundState());
}

QVector3D AbstractPhysicsCharacter::getGroundPosition() const
{
    if (character() == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(character()->GetGroundPosition());
}

QVector3D AbstractPhysicsCharacter::getGroundNormal() const
{
    if (character() == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(character()->GetGroundNormal());
}

QVector3D AbstractPhysicsCharacter::getGroundVelocity() const
{
    if (character() == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(character()->GetGroundVelocity());
}

AbstractPhysicsBody *AbstractPhysicsCharacter::getGroundBody() const
{
    if (character() == nullptr)
        return nullptr;

    JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(character()->GetGroundBodyID()));
    if (bodyLock.Succeeded()) {
        const JPH::Body &body = bodyLock.GetBody();
        return qobject_cast<AbstractPhysicsBody *>(reinterpret_cast<AbstractPhysicsNode *>(body.GetUserData()));
    }

    return nullptr;
}

void AbstractPhysicsCharacter::handleUpChanged()
{
    if (character() == nullptr)
        return;

    character()->SetUp(PhysicsUtils::toJoltType(up()));
}
