#include "charactervirtual_p.h"
#include "physicsutils_p.h"
#include "physicssystem_p.h"
#include "body_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/EmptyShape.h>

CharacterVirtual::CharacterVirtual(QQuick3DNode *parent) : AbstractPhysicsCharacter(parent)
{
}

CharacterVirtual::~CharacterVirtual()
{
    CharacterVirtual::cleanup();
}

bool CharacterVirtual::enhancedInternalEdgeRemoval() const
{
    return m_characterSettings.mEnhancedInternalEdgeRemoval;
}

void CharacterVirtual::setEnhancedInternalEdgeRemoval(bool enhancedInternalEdgeRemoval)
{
    if (m_characterSettings.mEnhancedInternalEdgeRemoval == enhancedInternalEdgeRemoval)
        return;

    m_characterSettings.mEnhancedInternalEdgeRemoval = enhancedInternalEdgeRemoval;
    if (m_character)
        m_character->SetEnhancedInternalEdgeRemoval(enhancedInternalEdgeRemoval);

    emit enhancedInternalEdgeRemovalChanged(enhancedInternalEdgeRemoval);
}

float CharacterVirtual::mass() const
{
    return m_characterSettings.mMass;
}

void CharacterVirtual::setMass(float mass)
{
    if (qFuzzyCompare(m_characterSettings.mMass, mass))
        return;

    m_characterSettings.mMass = mass;
    if (m_character)
        m_character->SetMass(mass);

    emit massChanged(mass);
}

float CharacterVirtual::maxStrength() const
{
    return m_characterSettings.mMaxStrength;
}

void CharacterVirtual::setMaxStrength(float maxStrength)
{
    if (qFuzzyCompare(m_characterSettings.mMaxStrength, maxStrength))
        return;

    m_characterSettings.mMaxStrength = maxStrength;
    if (m_character)
        m_character->SetMaxStrength(maxStrength);

    emit maxStrengthChanged(maxStrength);
}

QVector3D CharacterVirtual::shapeOffset() const
{
    return m_shapeOffset;
}

void CharacterVirtual::setShapeOffset(const QVector3D &shapeOffset)
{
    if (m_shapeOffset == shapeOffset)
        return;

    m_shapeOffset = shapeOffset;
    if (m_character)
        m_character->SetShapeOffset(PhysicsUtils::toJoltType(shapeOffset));

    emit shapeOffsetChanged(shapeOffset);
}

Quick3dJoltPhysics::BackFaceMode CharacterVirtual::backFaceMode() const
{
    return m_backFaceMode;
}

void CharacterVirtual::setBackFaceMode(Quick3dJoltPhysics::BackFaceMode backFaceMode)
{
    if (m_backFaceMode == backFaceMode)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'backFaceMode' after character is initialized will have "
                      "no effect";
        return;
    }

    m_backFaceMode = backFaceMode;
    emit backFaceModeChanged(backFaceMode);
}

float CharacterVirtual::predictiveContactDistance() const
{
    return m_characterSettings.mPredictiveContactDistance;
}

void CharacterVirtual::setPredictiveContactDistance(float predictiveContactDistance)
{
    if (qFuzzyCompare(m_characterSettings.mPredictiveContactDistance, predictiveContactDistance))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'predictiveContactDistance' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mPredictiveContactDistance = predictiveContactDistance;
    emit predictiveContactDistanceChanged(predictiveContactDistance);
}

quint32 CharacterVirtual::maxCollisionIterations() const
{
    return m_characterSettings.mMaxCollisionIterations;
}

void CharacterVirtual::setMaxCollisionIterations(quint32 maxCollisionIterations)
{
    if (m_characterSettings.mMaxCollisionIterations == maxCollisionIterations)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'maxCollisionIterations' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMaxCollisionIterations = maxCollisionIterations;
    emit maxCollisionIterationsChanged(maxCollisionIterations);
}

quint32 CharacterVirtual::maxConstraintIterations() const
{
    return m_characterSettings.mMaxConstraintIterations;
}

void CharacterVirtual::setMaxConstraintIterations(quint32 maxConstraintIterations)
{
    if (m_characterSettings.mMaxCollisionIterations == maxConstraintIterations)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'maxConstraintIterations' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMaxConstraintIterations = maxConstraintIterations;
    emit maxConstraintIterationsChanged(maxConstraintIterations);
}

float CharacterVirtual::minTimeRemaining() const
{
    return m_characterSettings.mMinTimeRemaining;
}

void CharacterVirtual::setMinTimeRemaining(float minTimeRemaining)
{
    if (qFuzzyCompare(m_characterSettings.mMinTimeRemaining, minTimeRemaining))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'minTimeRemaining' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMinTimeRemaining = minTimeRemaining;
    emit minTimeRemainingChanged(minTimeRemaining);
}

float CharacterVirtual::collisionTolerance() const
{
    return m_characterSettings.mCollisionTolerance;
}

void CharacterVirtual::setCollisionTolerance(float collisionTolerance)
{
    if (qFuzzyCompare(m_characterSettings.mCollisionTolerance, collisionTolerance))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'collisionTolerance' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mCollisionTolerance = collisionTolerance;
    emit collisionToleranceChanged(collisionTolerance);
}

float CharacterVirtual::characterPadding() const
{
    return m_characterSettings.mCharacterPadding;
}

void CharacterVirtual::setCharacterPadding(float characterPadding)
{
    if (qFuzzyCompare(m_characterSettings.mCharacterPadding, characterPadding))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'characterPadding' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mCharacterPadding = characterPadding;
    emit characterPaddingChanged(characterPadding);
}

quint32 CharacterVirtual::maxNumHits() const
{
    return m_characterSettings.mMaxNumHits;
}

void CharacterVirtual::setMaxNumHits(quint32 maxNumHits)
{
    if (m_characterSettings.mMaxNumHits == maxNumHits)
        return;

    m_characterSettings.mMaxNumHits = maxNumHits;
    if (m_character)
        m_character->SetMaxNumHits(maxNumHits);

    emit maxNumHitsChanged(maxNumHits);
}

float CharacterVirtual::hitReductionCosMaxAngle() const
{
    return m_characterSettings.mHitReductionCosMaxAngle;
}

void CharacterVirtual::setHitReductionCosMaxAngle(float hitReductionCosMaxAngle)
{
    if (qFuzzyCompare(m_characterSettings.mHitReductionCosMaxAngle, hitReductionCosMaxAngle))
        return;

    m_characterSettings.mHitReductionCosMaxAngle = hitReductionCosMaxAngle;
    if (m_character)
        m_character->SetHitReductionCosMaxAngle(hitReductionCosMaxAngle);

    emit hitReductionCosMaxAngleChanged(hitReductionCosMaxAngle);
}

float CharacterVirtual::penetrationRecoverySpeed() const
{
    return m_characterSettings.mPenetrationRecoverySpeed;
}

void CharacterVirtual::setPenetrationRecoverySpeed(float penetrationRecoverySpeed)
{
    if (qFuzzyCompare(m_characterSettings.mPenetrationRecoverySpeed, penetrationRecoverySpeed))
        return;

    m_characterSettings.mPenetrationRecoverySpeed = penetrationRecoverySpeed;
    if (m_character)
        m_character->SetPenetrationRecoverySpeed(penetrationRecoverySpeed);

    emit penetrationRecoverySpeedChanged(penetrationRecoverySpeed);
}

AbstractCharacterContactListener *CharacterVirtual::characterContactListener() const
{
    return m_characterContactListener;
}

void CharacterVirtual::setCharacterContactListener(AbstractCharacterContactListener *characterContactListener)
{
    if (m_characterContactListener == characterContactListener)
        return;

    if (m_characterContactListener)
        m_characterContactListener->disconnect(this);

    m_characterContactListener = characterContactListener;

    if (m_characterContactListener) {
        connect(m_characterContactListener, &QObject::destroyed, this,
                [this] (QObject *obj) 
        { 
            if (m_characterContactListener == obj)
                setCharacterContactListener(nullptr); 
        });
    }

    if (m_character) {
        if (m_characterContactListener)
            m_character->SetListener(m_characterContactListener->getJoltCharacterContactListener());
        else
            m_character->SetListener(nullptr);
    }

    emit characterContactListenerChanged(m_characterContactListener);
}

int CharacterVirtual::innerBodyShapeLayer() const
{
    return m_characterSettings.mInnerBodyLayer;
}

void CharacterVirtual::setInnerBodyShapeLayer(int innerShapeLayer)
{
    if (m_characterSettings.mInnerBodyLayer == innerShapeLayer)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'innerShapeLayer' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mInnerBodyLayer = innerShapeLayer;
    emit innerBodyShapeLayerChanged(innerShapeLayer);
}

AbstractShape *CharacterVirtual::innerBodyShape() const
{
    return m_innerBodyShape;
}

void CharacterVirtual::setInnerBodyShape(AbstractShape *innerBodyShape)
{
    if (m_innerBodyShape == innerBodyShape)
        return;

    if (m_innerBodyShape)
        PhysicsSystem::removeFromCharacterVsCharacterCollision(this);

    QQuick3DObjectPrivate::attachWatcher(this, &CharacterVirtual::setInnerBodyShape, innerBodyShape, m_innerBodyShape);
    m_innerBodyShape = innerBodyShape;
    if (m_innerBodyShape && m_innerBodyShape->parentItem() == nullptr)
        m_innerBodyShape->setParentItem(this);

    if (m_character) {
        if (m_innerBodyShape) {
            JPH::Ref<JPH::Shape> joltInnerBodyShape = m_innerBodyShape->getJoltShape();
            if (!joltInnerBodyShape->MustBeStatic()) {
                m_character->SetInnerBodyShape(joltInnerBodyShape);
                PhysicsSystem::addToCharacterVsCharacterCollision(this);
            } else {
                qWarning() << "CharacterVirtual: Cannot make character containing static inner body shape.";
            }
        } else {
            m_character->SetInnerBodyShape(new JPH::EmptyShape());
        }
    }

    emit innerBodyShapeChanged(m_innerBodyShape);
}

QVector3D CharacterVirtual::getLinearVelocity() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetLinearVelocity());
}

void CharacterVirtual::setLinearVelocity(const QVector3D &linearVelocity)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setLinearVelocity' before character is initialized will have no effect";
        return;
    }

    m_character->SetLinearVelocity(PhysicsUtils::toJoltType(linearVelocity));
}

void CharacterVirtual::updateGroundVelocity()
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'updateGroundVelocity' before character is initialized will have no effect";
        return;
    }

    m_character->UpdateGroundVelocity();
}

void CharacterVirtual::teleport(const QVector3D &position, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'teleport' before character is initialized will have no effect";
        return;
    }

    m_character->SetPosition(PhysicsUtils::toJoltType(position));

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    m_character->RefreshContacts(m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                                 m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                 bf,
                                 {},
                                 *m_tempAllocator);
}

void CharacterVirtual::extendedUpdate(float deltaTime, const QVector3D &gravity, ExtendedUpdateSettings *updateSettings, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'extendedUpdate' before character is initialized will have no effect";
        return;
    }

    JPH::CharacterVirtual::ExtendedUpdateSettings inSettings;
    if (updateSettings) {
        inSettings.mStickToFloorStepDown = PhysicsUtils::toJoltType(updateSettings->stickToFloorStepDown);
        inSettings.mWalkStairsStepUp = PhysicsUtils::toJoltType(updateSettings->walkStairsStepUp);
        inSettings.mWalkStairsMinStepForward = updateSettings->walkStairsMinStepForward;
        inSettings.mWalkStairsStepForwardTest = updateSettings->walkStairsStepForwardTest;
        inSettings.mWalkStairsCosAngleForwardContact = updateSettings->walkStairsCosAngleForwardContact;
        inSettings.mWalkStairsStepDownExtra = PhysicsUtils::toJoltType(updateSettings->walkStairsStepDownExtra);
    }

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    m_character->ExtendedUpdate(deltaTime,
                                PhysicsUtils::toJoltType(gravity),
                                inSettings,
                                m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                                m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                bf,
                                {},
                                *m_tempAllocator);

    QVector3D position = PhysicsUtils::toQtType(m_character->GetCenterOfMassTransform().GetColumn3(3));
    const QQuick3DNode *parentNode = static_cast<QQuick3DNode *>(parentItem());

    if (!parentNode) {
        QQuick3DNode::setPosition(position);
    } else {
        QQuick3DNode::setPosition(parentNode->mapPositionFromScene(position));
    }

    emitContactCallbacks();
}

bool CharacterVirtual::setShape(float maxPenetrationDepth, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setShape' before character is initialized will have no effect";
        return false;
    }

    if (m_shape == nullptr)
        return false;

    auto shape = m_shape->getJoltShape();

    if (shape->MustBeStatic()) {
        qWarning() << "CharacterVirtual: Cannot make character containing static shape.";
        return false;
    }

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    return m_character->SetShape(shape,
                                 maxPenetrationDepth,
                                 m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                                 m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                 bf,
                                 {},
                                 *m_tempAllocator);
}

JPH::CharacterBase *CharacterVirtual::character() const
{
    return m_character;
}

void CharacterVirtual::updateJoltObject()
{
    if (m_character || m_jolt == nullptr || m_shape == nullptr)
        return;

    auto shape = m_shape->getJoltShape();
    if (shape->MustBeStatic()) {
        qWarning() << "CharacterVirtual: Cannot make character containing static shape.";
        return;
    }

    JPH::Ref<JPH::Shape> joltInnerBodyShape = m_innerBodyShape ? m_innerBodyShape->getJoltShape() : new JPH::EmptyShape();
    if (joltInnerBodyShape->MustBeStatic()) {
        qWarning() << "CharacterVirtual: Cannot make character containing static inner body shape.";
        return;
    }

    m_characterSettings.mShape = shape;
    m_characterSettings.mSupportingVolume = JPH::Plane(PhysicsUtils::toJoltType(supportingVolume().toVector3D()), supportingVolume().w());
    m_characterSettings.mBackFaceMode = static_cast<JPH::EBackFaceMode>(m_backFaceMode);
    m_characterSettings.mMaxSlopeAngle = qDegreesToRadians(maxSlopeAngle());
    m_characterSettings.mUp = PhysicsUtils::toJoltType(up());

    m_characterSettings.mInnerBodyShape = joltInnerBodyShape;

    m_character = new JPH::CharacterVirtual(
        &m_characterSettings, PhysicsUtils::toJoltType(scenePosition()), PhysicsUtils::toJoltType(sceneRotation()), 0, m_jolt);

    m_character->SetUserData(reinterpret_cast<JPH::uint64>(this));

    if (m_innerBodyShape)
        PhysicsSystem::addToCharacterVsCharacterCollision(this);

    if (m_characterContactListener)
        m_character->SetListener(m_characterContactListener->getJoltCharacterContactListener());

    m_bodyID = m_character->GetInnerBodyID().GetIndexAndSequenceNumber();
    emit bodyIDChanged(m_bodyID);
}

void CharacterVirtual::cleanup()
{
    if (m_character) {
        PhysicsSystem::removeFromCharacterVsCharacterCollision(this);
        delete m_character;
    }
    m_character = nullptr;
}

void CharacterVirtual::emitContactCallbacks()
{
    if (m_characterContactListener == nullptr)
        return;

    for (const auto &contact : std::as_const(m_characterContactListener->m_contacts)) {
        Body *qtBody = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(contact.bodyID2));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody) {
            emit qtBody->characterContact(this, contact.contactPosition, contact.contactNormal);
            emit characterContact(qtBody, contact.contactPosition, -contact.contactNormal);
        }
    }

    m_characterContactListener->m_contacts.clear();
}
