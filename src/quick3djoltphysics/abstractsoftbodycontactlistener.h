#ifndef ABSTRACTSOFTBODYCONTACTLISTENER_H
#define ABSTRACTSOFTBODYCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>
#include <QObject>
#include <QVector3D>

namespace JPH {
class Body;
class SoftBodyManifold;
class SoftBodyContactListener;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractSoftBodyContactListener : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SoftBodyContactListener)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractSoftBodyContactListener(QObject *parent = nullptr);
    ~AbstractSoftBodyContactListener() override;

    enum class ValidateResult {
        AcceptContact,
        RejectContact
    };
    Q_ENUM(ValidateResult)

    struct SoftBodyContactSettings {
        float invMassScale1 = 1.0f;
        float invMassScale2 = 1.0f;
        float invInertiaScale2 = 1.0f;
        bool isSensor = false;
    };

    virtual ValidateResult softBodyContactValidate(quint32 softBodyID,
                                                   quint32 otherBodyID,
                                                   SoftBodyContactSettings &settings) = 0;
    virtual void softBodyContactAdded(quint32 softBodyID, int numVerticesInContact) = 0;

private:
    JPH::SoftBodyContactListener *getJoltSoftBodyContactListener() const;

    friend class SoftBodyContactListenerImpl;
    friend class PhysicsSystem;

    JPH::SoftBodyContactListener *m_impl = nullptr;
};

#endif // ABSTRACTSOFTBODYCONTACTLISTENER_H
