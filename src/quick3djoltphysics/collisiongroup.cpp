#include "collisiongroup_p.h"

CollisionGroup::CollisionGroup(QObject *parent) : QObject(parent)
{
}

CollisionGroup::~CollisionGroup() = default;

GroupFilterTable *CollisionGroup::groupFilterTable() const
{
    return m_groupFilterTable;
}

void CollisionGroup::setGroupFilterTable(GroupFilterTable *groupFilterTable)
{
    if (m_groupFilterTable == groupFilterTable)
        return;

    if (m_groupFilterTable)
        m_groupFilterTable->disconnect(this);

    m_groupFilterTable = groupFilterTable;

    if (m_groupFilterTable) {
        QObject::connect(m_groupFilterTable, &GroupFilterTable::changed, this,
                         [this] { handleGroupFilterTableChange(); });
        QObject::connect(m_groupFilterTable, &QObject::destroyed, this,
                         [this](QObject *obj)
                         {
                             if (m_groupFilterTable == obj)
                                 setGroupFilterTable(nullptr);
                         });
    }

    emit groupFilterTableChanged(m_groupFilterTable);
    handleGroupFilterTableChange();
}

quint32 CollisionGroup::groupId() const
{
    return m_groupId;
}

void CollisionGroup::setGroupId(quint32 groupId)
{
    if (m_groupId == groupId)
        return;

    m_groupId = groupId;
    m_collisionGroup.SetGroupID(m_groupId);

    emit groupIdChanged(m_groupId);
    emit changed();
}

quint32 CollisionGroup::subGroupId() const
{
    return m_subGroupId;
}

void CollisionGroup::setSubGroupId(quint32 subGroupId)
{
    if (m_subGroupId == subGroupId)
        return;

    m_subGroupId = subGroupId;
    m_collisionGroup.SetSubGroupID(m_subGroupId);

    emit subGroupIdChanged(m_subGroupId);
    emit changed();
}

JPH::CollisionGroup CollisionGroup::getJoltCollisionGroup() const
{
    return m_collisionGroup;
}

void CollisionGroup::handleGroupFilterTableChange()
{
    JPH::Ref<JPH::GroupFilterTable> filter;
    if (m_groupFilterTable != nullptr)
        filter = m_groupFilterTable->getJoltGroupFilterTable();
    m_collisionGroup.SetGroupFilter(filter);
    emit changed();
}
