#include "abstractcharactercontactlistener.h"
#include "charactervirtual_p.h"
#include "physicsutils_p.h"

#include <QtGlobal>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Body/Body.h>

static inline AbstractCharacterContactListener::CharacterContactSettings toCharacterContactSettings(const JPH::CharacterContactSettings &ioSettings)
{
    AbstractCharacterContactListener::CharacterContactSettings settings;
    settings.canPushCharacter = ioSettings.mCanPushCharacter;
    settings.canReceiveImpulses = ioSettings.mCanReceiveImpulses;
    return settings;
}

static inline JPH::CharacterContactSettings toJoltCharacterContactSettings(const AbstractCharacterContactListener::CharacterContactSettings &settings)
{
    JPH::CharacterContactSettings ioSettings;
    ioSettings.mCanPushCharacter = settings.canPushCharacter;
    ioSettings.mCanReceiveImpulses = settings.canReceiveImpulses;
    return ioSettings;
}

static inline AbstractCharacterContactListener::Contact toBodyContact(const JPH::CharacterContact &inContact)
{
    AbstractCharacterContactListener::Contact contact;
    contact.bodyID2 = inContact.mBodyB.GetIndexAndSequenceNumber();
    contact.contactPosition = PhysicsUtils::toQtType(inContact.mPosition);
    contact.contactNormal = PhysicsUtils::toQtType(inContact.mContactNormal);
    return contact;
}

static inline AbstractCharacterContactListener::Contact toCharacterContact(const JPH::CharacterContact &inContact)
{
    auto *otherCharacter = inContact.mCharacterB != nullptr
        ? reinterpret_cast<CharacterVirtual *>(inContact.mCharacterB->GetUserData())
        : nullptr;
    Q_ASSERT(otherCharacter);

    AbstractCharacterContactListener::Contact contact;
    contact.bodyID2 = otherCharacter->bodyID();
    contact.contactPosition = PhysicsUtils::toQtType(inContact.mPosition);
    contact.contactNormal = PhysicsUtils::toQtType(inContact.mContactNormal);
    return contact;
}

class CharacterContactListenerImpl : public JPH::CharacterContactListener
{
public:
    CharacterContactListenerImpl(AbstractCharacterContactListener *d) : m_d(d) {}

    void OnAdjustBodyVelocity(const JPH::CharacterVirtual *inCharacter, const JPH::Body &inBody2, JPH::Vec3 &ioLinearVelocity, JPH::Vec3 &ioAngularVelocity) override
    {
        Q_UNUSED(inCharacter);

        auto linearVelocity = PhysicsUtils::toQtType(ioLinearVelocity);
        auto angularVelocity = PhysicsUtils::toQtType(ioAngularVelocity);

        m_d->onAdjustBodyVelocity(inBody2.GetID().GetIndexAndSequenceNumber(), linearVelocity, angularVelocity);

        ioLinearVelocity = PhysicsUtils::toJoltType(linearVelocity);
        ioAngularVelocity = PhysicsUtils::toJoltType(angularVelocity);
    }

    void OnContactAdded(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterContact &inContact, JPH::CharacterContactSettings &ioSettings) override
    {
        Q_UNUSED(inCharacter);

        auto settings = toCharacterContactSettings(ioSettings);
        m_d->onContactAdded(toBodyContact(inContact), settings);
        ioSettings = toJoltCharacterContactSettings(settings);
    }

    void OnContactPersisted(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterContact &inContact, JPH::CharacterContactSettings &ioSettings) override
    {
        Q_UNUSED(inCharacter);

        auto settings = toCharacterContactSettings(ioSettings);
        m_d->onContactAdded(toBodyContact(inContact), settings);
        ioSettings = toJoltCharacterContactSettings(settings);
    }

    void OnContactSolve(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3 &ioNewCharacterVelocity) override
    {
        Q_UNUSED(inContactMaterial);
        Q_UNUSED(inSubShapeID2);

        const bool isSlopeTooSteep = inCharacter->IsSlopeTooSteep(inContactNormal);

        auto newCharacterVelocity = PhysicsUtils::toQtType(ioNewCharacterVelocity);

        AbstractCharacterContactListener::Contact contact;
        contact.bodyID2 = inBodyID2.GetIndexAndSequenceNumber();
        contact.contactPosition = PhysicsUtils::toQtType(inContactPosition);
        contact.contactNormal = PhysicsUtils::toQtType(inContactNormal);

        m_d->onContactSolve(contact,
                            PhysicsUtils::toQtType(inContactVelocity),
                            PhysicsUtils::toQtType(inCharacterVelocity),
                            newCharacterVelocity,
                            isSlopeTooSteep);

        ioNewCharacterVelocity = PhysicsUtils::toJoltType(newCharacterVelocity);
    }

    void OnCharacterContactAdded(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterContact &inContact, JPH::CharacterContactSettings &ioSettings) override
    {
        Q_UNUSED(inCharacter);

        auto settings = toCharacterContactSettings(ioSettings);
        m_d->onCharacterContactAdded(toCharacterContact(inContact), settings);
        ioSettings = toJoltCharacterContactSettings(settings);
    }

    void OnCharacterContactPersisted(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterContact &inContact, JPH::CharacterContactSettings &ioSettings) override
    {
        Q_UNUSED(inCharacter);

        auto settings = toCharacterContactSettings(ioSettings);
        m_d->onCharacterContactAdded(toCharacterContact(inContact), settings);
        ioSettings = toJoltCharacterContactSettings(settings);
    }

    void OnCharacterContactSolve(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterVirtual *inOtherCharacter, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3 &ioNewCharacterVelocity) override
    {
        Q_UNUSED(inCharacter);
        Q_UNUSED(inContactMaterial);
        Q_UNUSED(inSubShapeID2);

        auto newCharacterVelocity = PhysicsUtils::toQtType(ioNewCharacterVelocity);

        auto *otherCharacter = reinterpret_cast<CharacterVirtual *>(inOtherCharacter->GetUserData());
        Q_ASSERT(otherCharacter);

        AbstractCharacterContactListener::Contact contact;
        contact.bodyID2 = otherCharacter->bodyID();
        contact.contactPosition = PhysicsUtils::toQtType(inContactPosition);
        contact.contactNormal = PhysicsUtils::toQtType(inContactNormal);

        m_d->onCharacterContactSolve(contact,
                                     PhysicsUtils::toQtType(inContactVelocity),
                                     PhysicsUtils::toQtType(inCharacterVelocity),
                                     newCharacterVelocity);

        ioNewCharacterVelocity = PhysicsUtils::toJoltType(newCharacterVelocity);
    }

private:
    AbstractCharacterContactListener *m_d = nullptr;
};

AbstractCharacterContactListener::AbstractCharacterContactListener(QObject *parent)
    : QObject(parent)
    , m_impl(new CharacterContactListenerImpl(this))
{
}

AbstractCharacterContactListener::~AbstractCharacterContactListener()
{
    delete m_impl;
    m_impl = nullptr;
}

void AbstractCharacterContactListener::registerContact(const Contact &contact)
{
    QMutexLocker locker(&m_mutex);
    m_contacts.push_back(contact);
}

JPH::CharacterContactListener *AbstractCharacterContactListener::getJoltCharacterContactListener() const
{
    return m_impl;
}
