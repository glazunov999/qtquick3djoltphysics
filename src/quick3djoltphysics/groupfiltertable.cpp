#include "groupfiltertable_p.h"

GroupFilterTable::GroupFilterTable(QObject *parent) : QObject(parent)
{
}

GroupFilterTable::~GroupFilterTable() = default;

void GroupFilterTable::componentComplete()
{
    createJoltGroupFilterTable();
}

quint32 GroupFilterTable::numSubGroups() const
{
    return m_numSubGroups;
}

void GroupFilterTable::setNumSubGroups(quint32 numSubGroups)
{
    if (m_numSubGroups == numSubGroups)
        return;

    m_numSubGroups = numSubGroups;
    if (m_groupFilterTable) {
        m_groupFilterTable = nullptr;
        createJoltGroupFilterTable();
    }

    emit numSubGroupsChanged(m_numSubGroups);
    emit changed();
}

void GroupFilterTable::disableCollision(quint32 subGroup1, quint32 subGroup2)
{
    if (!m_groupFilterTable) {
        qWarning() << "Warning: calling 'disableCollision' before GroupFilterTable is complete will have "
                      "no effect";
        return;
    }

    m_groupFilterTable->DisableCollision(subGroup1, subGroup2);
}

void GroupFilterTable::enableCollision(quint32 subGroup1, quint32 subGroup2)
{
    if (!m_groupFilterTable) {
        qWarning() << "Warning: calling 'enableCollision' before GroupFilterTable is complete will have "
                      "no effect";
        return;
    }

    m_groupFilterTable->EnableCollision(subGroup1, subGroup2);
}

void GroupFilterTable::createJoltGroupFilterTable()
{
    if (m_groupFilterTable)
        return;

    m_groupFilterTable = new JPH::GroupFilterTable(m_numSubGroups);
}

JPH::Ref<JPH::GroupFilterTable> GroupFilterTable::getJoltGroupFilterTable() const
{
    return m_groupFilterTable;
}
