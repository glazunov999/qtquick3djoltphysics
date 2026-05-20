#include "examplesoftbodycontactlistener.h"

ExampleSoftBodyContactListener::ExampleSoftBodyContactListener(QObject *parent)
    : AbstractSoftBodyContactListener(parent)
{
}

int ExampleSoftBodyContactListener::cycle() const
{
    return m_cycle;
}

void ExampleSoftBodyContactListener::setCycle(int cycle)
{
    const int clamped = ((cycle % 10) + 10) % 10;
    if (m_cycle == clamped)
        return;
    m_cycle = clamped;
    if (m_contactVertexCount != 0) {
        m_contactVertexCount = 0;
        emit contactVertexCountChanged();
    }
    emit cycleChanged();
}

int ExampleSoftBodyContactListener::contactVertexCount() const
{
    return m_contactVertexCount;
}

ExampleSoftBodyContactListener::ValidateResult ExampleSoftBodyContactListener::softBodyContactValidate(
    quint32 softBodyID,
    quint32 otherBodyID,
    SoftBodyContactSettings &settings)
{
    Q_UNUSED(softBodyID)
    Q_UNUSED(otherBodyID)

    switch (m_cycle) {
    case 0:
        return ValidateResult::AcceptContact;

    case 1:
        settings.invMassScale2 = 0.1f;
        settings.invInertiaScale2 = 0.1f;
        return ValidateResult::AcceptContact;

    case 2:
        settings.invMassScale1 = 0.1f;
        return ValidateResult::AcceptContact;

    case 3:
        settings.invMassScale2 = 0.0f;
        settings.invInertiaScale2 = 0.0f;
        return ValidateResult::AcceptContact;

    case 4:
        settings.invMassScale1 = 0.0f;
        return ValidateResult::AcceptContact;

    case 5:
        settings.isSensor = true;
        return ValidateResult::AcceptContact;

    case 6:
        return ValidateResult::RejectContact;

    case 7:
        return ValidateResult::AcceptContact;

    case 8:
        settings.invMassScale1 = 0.0f;
        return ValidateResult::AcceptContact;

    case 9:
        settings.isSensor = true;
        return ValidateResult::AcceptContact;

    default:
        return ValidateResult::RejectContact;
    }
}

void ExampleSoftBodyContactListener::softBodyContactAdded(quint32 softBodyID, int numVerticesInContact)
{
    Q_UNUSED(softBodyID)

    if (m_contactVertexCount == numVerticesInContact)
        return;
    m_contactVertexCount = numVerticesInContact;
    emit contactVertexCountChanged();
}
