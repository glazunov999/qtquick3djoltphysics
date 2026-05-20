#include "abstractsoftbodycontactlistener.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/SoftBody/SoftBodyContactListener.h>
#include <Jolt/Physics/SoftBody/SoftBodyManifold.h>

class SoftBodyContactListenerImpl : public JPH::SoftBodyContactListener
{
public:
    explicit SoftBodyContactListenerImpl(AbstractSoftBodyContactListener *owner)
        : m_owner(owner) {}

    JPH::SoftBodyValidateResult OnSoftBodyContactValidate(
        const JPH::Body &inSoftBody,
        const JPH::Body &inOtherBody,
        JPH::SoftBodyContactSettings &ioSettings) override
    {
        AbstractSoftBodyContactListener::SoftBodyContactSettings settings;
        settings.invMassScale1 = ioSettings.mInvMassScale1;
        settings.invMassScale2 = ioSettings.mInvMassScale2;
        settings.invInertiaScale2 = ioSettings.mInvInertiaScale2;
        settings.isSensor = ioSettings.mIsSensor;

        const auto result = m_owner->softBodyContactValidate(
            inSoftBody.GetID().GetIndexAndSequenceNumber(),
            inOtherBody.GetID().GetIndexAndSequenceNumber(),
            settings);

        ioSettings.mInvMassScale1 = settings.invMassScale1;
        ioSettings.mInvMassScale2 = settings.invMassScale2;
        ioSettings.mInvInertiaScale2 = settings.invInertiaScale2;
        ioSettings.mIsSensor = settings.isSensor;

        return (result == AbstractSoftBodyContactListener::ValidateResult::AcceptContact)
                   ? JPH::SoftBodyValidateResult::AcceptContact
                   : JPH::SoftBodyValidateResult::RejectContact;
    }

    void OnSoftBodyContactAdded(
        const JPH::Body &inSoftBody,
        const JPH::SoftBodyManifold &inManifold) override
    {
        int numInContact = 0;
        for (const auto &vertex : inManifold.GetVertices()) {
            if (inManifold.HasContact(vertex))
                ++numInContact;
        }

        m_owner->softBodyContactAdded(
            inSoftBody.GetID().GetIndexAndSequenceNumber(),
            numInContact);
    }

private:
    AbstractSoftBodyContactListener *m_owner = nullptr;
};

AbstractSoftBodyContactListener::AbstractSoftBodyContactListener(QObject *parent)
    : QObject(parent)
    , m_impl(new SoftBodyContactListenerImpl(this))
{
}

AbstractSoftBodyContactListener::~AbstractSoftBodyContactListener()
{
    delete m_impl;
}

JPH::SoftBodyContactListener *AbstractSoftBodyContactListener::getJoltSoftBodyContactListener() const
{
    return m_impl;
}
