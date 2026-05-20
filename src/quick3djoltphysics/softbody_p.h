#ifndef SOFTBODY_P_H
#define SOFTBODY_P_H

#include "abstractphysicsnode_p.h"
#include "softbodysharedsettings.h"

class SoftBodyMeshGeometry;

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/SoftBody/SoftBodyCreationSettings.h>

#include <QSet>
#include <QVector3D>
#include <QQuaternion>

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBody : public AbstractPhysicsNode
{
    Q_OBJECT
    Q_PROPERTY(SoftBodySharedSettings *sharedSettings READ sharedSettings WRITE setSharedSettings NOTIFY sharedSettingsChanged)
    Q_PROPERTY(quint32 objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(int numIterations READ numIterations WRITE setNumIterations NOTIFY numIterationsChanged)
    Q_PROPERTY(float linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged)
    Q_PROPERTY(float maxLinearVelocity READ maxLinearVelocity WRITE setMaxLinearVelocity NOTIFY maxLinearVelocityChanged)
    Q_PROPERTY(float restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(float friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(float pressure READ pressure WRITE setPressure NOTIFY pressureChanged)
    Q_PROPERTY(float gravityFactor READ gravityFactor WRITE setGravityFactor NOTIFY gravityFactorChanged)
    Q_PROPERTY(bool allowSleeping READ allowSleeping WRITE setAllowSleeping NOTIFY allowSleepingChanged)
    Q_PROPERTY(bool updatePosition READ updatePosition WRITE setUpdatePosition NOTIFY updatePositionChanged)
    Q_PROPERTY(bool makeRotationIdentity READ makeRotationIdentity WRITE setMakeRotationIdentity NOTIFY makeRotationIdentityChanged)
    Q_PROPERTY(quint32 bodyID READ bodyID NOTIFY bodyIDChanged)
    QML_NAMED_ELEMENT(SoftBody)
public:
    explicit SoftBody(QQuick3DNode *parent = nullptr);
    ~SoftBody() override;

    SoftBodySharedSettings *sharedSettings() const;
    void setSharedSettings(SoftBodySharedSettings *settings);

    quint32 objectLayer() const;
    void setObjectLayer(quint32 objectLayer);

    int numIterations() const;
    void setNumIterations(int numIterations);

    float linearDamping() const;
    void setLinearDamping(float linearDamping);

    float maxLinearVelocity() const;
    void setMaxLinearVelocity(float maxLinearVelocity);

    float restitution() const;
    void setRestitution(float restitution);

    float friction() const;
    void setFriction(float friction);

    float pressure() const;
    void setPressure(float pressure);

    float gravityFactor() const;
    void setGravityFactor(float gravityFactor);

    bool allowSleeping() const;
    void setAllowSleeping(bool allowSleeping);

    bool updatePosition() const;
    void setUpdatePosition(bool updatePosition);

    bool makeRotationIdentity() const;
    void setMakeRotationIdentity(bool makeRotationIdentity);

    quint32 bodyID() const;

    Q_INVOKABLE QVector3D centerOfMassPosition() const;

    JPH::Body *joltBody() const { return m_body; }

    void registerMeshGeometry(SoftBodyMeshGeometry *geometry);
    void unregisterMeshGeometry(SoftBodyMeshGeometry *geometry);

    Q_INVOKABLE void setPressureRuntime(float pressure);
    Q_INVOKABLE void setNumIterationsRuntime(int numIterations);
    Q_INVOKABLE void setVertexVelocity(int vertexIndex, const QVector3D &velocity);
    Q_INVOKABLE void addForce(const QVector3D &force);

signals:
    void sharedSettingsChanged(SoftBodySharedSettings *settings);
    void objectLayerChanged(quint32 objectLayer);
    void numIterationsChanged(int numIterations);
    void linearDampingChanged(float linearDamping);
    void maxLinearVelocityChanged(float maxLinearVelocity);
    void restitutionChanged(float restitution);
    void frictionChanged(float friction);
    void pressureChanged(float pressure);
    void gravityFactorChanged(float gravityFactor);
    void allowSleepingChanged(bool allowSleeping);
    void updatePositionChanged(bool updatePosition);
    void makeRotationIdentityChanged(bool makeRotationIdentity);
    void bodyIDChanged(quint32 bodyID);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;

private:
    void syncMeshGeometries();

    SoftBodySharedSettings *m_sharedSettings = nullptr;
    quint32 m_objectLayer = 0;
    int m_numIterations = 5;
    float m_linearDamping = 0.1f;
    float m_maxLinearVelocity = 500.0f;
    float m_restitution = 0.0f;
    float m_friction = 0.2f;
    float m_pressure = 0.0f;
    float m_gravityFactor = 1.0f;
    bool m_allowSleeping = true;
    bool m_updatePosition = true;
    bool m_makeRotationIdentity = true;
    quint32 m_bodyID = 0;

    JPH::Body *m_body = nullptr;
    QSet<SoftBodyMeshGeometry *> m_meshGeometries;
};

#endif // SOFTBODY_P_H
