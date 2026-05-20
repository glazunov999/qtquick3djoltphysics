#ifndef COLLISIONGROUP_P_H
#define COLLISIONGROUP_P_H

#include "groupfiltertable_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/CollisionGroup.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT CollisionGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GroupFilterTable *groupFilterTable READ groupFilterTable WRITE setGroupFilterTable NOTIFY groupFilterTableChanged)
    Q_PROPERTY(quint32 groupId READ groupId WRITE setGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(quint32 subGroupId READ subGroupId WRITE setSubGroupId NOTIFY subGroupIdChanged)
    QML_NAMED_ELEMENT(CollisionGroup)
public:
    explicit CollisionGroup(QObject *parent = nullptr);
    ~CollisionGroup() override;

    GroupFilterTable *groupFilterTable() const;
    void setGroupFilterTable(GroupFilterTable *groupFilterTable);
    quint32 groupId() const;
    void setGroupId(quint32 groupId);
    quint32 subGroupId() const;
    void setSubGroupId(quint32 subGroupId);

signals:
    void groupFilterTableChanged(GroupFilterTable *groupFilterTable);
    void groupIdChanged(quint32 groupId);
    void subGroupIdChanged(quint32 subGroupId);
    void changed();

private:
    JPH::CollisionGroup getJoltCollisionGroup() const;
    void handleGroupFilterTableChange();

    friend class Body;

    GroupFilterTable *m_groupFilterTable = nullptr;
    quint32 m_groupId = ~quint32(0);
    quint32 m_subGroupId = ~quint32(0);
    JPH::CollisionGroup m_collisionGroup;
};

#endif // COLLISIONGROUP_P_H
