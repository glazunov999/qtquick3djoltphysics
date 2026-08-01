#ifndef GROUPFILTERTABLE_P_H
#define GROUPFILTERTABLE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT GroupFilterTable : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(quint32 numSubGroups READ numSubGroups WRITE setNumSubGroups NOTIFY numSubGroupsChanged)
    QML_NAMED_ELEMENT(GroupFilterTable)
public:
    explicit GroupFilterTable(QObject *parent = nullptr);
    ~GroupFilterTable() override;

    void classBegin() override {}
    void componentComplete() override;

    quint32 numSubGroups() const;
    void setNumSubGroups(quint32 numSubGroups);

    Q_INVOKABLE void disableCollision(quint32 subGroup1, quint32 subGroup2);
    Q_INVOKABLE void enableCollision(quint32 subGroup1, quint32 subGroup2);

signals:
    void numSubGroupsChanged(quint32 numSubGroups);
    void changed();

private:
    void createJoltGroupFilterTable();
    JPH::Ref<JPH::GroupFilterTable> getJoltGroupFilterTable() const;
    friend class CollisionGroup;

    quint32 m_numSubGroups = 0;
    JPH::Ref<JPH::GroupFilterTable> m_groupFilterTable = nullptr;
};

#endif // GROUPFILTERTABLE_P_H
