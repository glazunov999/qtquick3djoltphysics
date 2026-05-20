#ifndef EXAMPLESOFTBODYCONTACTLISTENER_H
#define EXAMPLESOFTBODYCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/abstractsoftbodycontactlistener.h>

class ExampleSoftBodyContactListener : public AbstractSoftBodyContactListener
{
    Q_OBJECT
    Q_PROPERTY(int cycle READ cycle WRITE setCycle NOTIFY cycleChanged)
    Q_PROPERTY(int contactVertexCount READ contactVertexCount NOTIFY contactVertexCountChanged)
public:
    explicit ExampleSoftBodyContactListener(QObject *parent = nullptr);

    int cycle() const;
    void setCycle(int cycle);

    int contactVertexCount() const;

    ValidateResult softBodyContactValidate(quint32 softBodyID,
                                           quint32 otherBodyID,
                                           SoftBodyContactSettings &settings) override;
    void softBodyContactAdded(quint32 softBodyID, int numVerticesInContact) override;

signals:
    void cycleChanged();
    void contactVertexCountChanged();

private:
    int m_cycle = 0;
    int m_contactVertexCount = 0;
};

#endif // EXAMPLESOFTBODYCONTACTLISTENER_H
