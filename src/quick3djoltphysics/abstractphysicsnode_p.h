#ifndef ABSTRACTPHYSICSNODE_P_H
#define ABSTRACTPHYSICSNODE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>

namespace JPH {
class PhysicsSystem;
class TempAllocator;
class BodyInterface;
}

class PhysicsSystem;

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsNode : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(Activation activation READ activation WRITE setActivation NOTIFY activationChanged)
    QML_NAMED_ELEMENT(PhysicsNode)
    QML_UNCREATABLE("abstract interface")
public:
    enum class Activation {
        Activate,
        DontActivate,
    };
    Q_ENUM(Activation)

    explicit AbstractPhysicsNode(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsNode() override;

    Activation activation() const;
    void setActivation(Activation activation);

signals:
    void activationChanged(Activation activation);

protected:
    void componentComplete() override;

    virtual void updateJoltObject() = 0;
    virtual void cleanup();
    virtual void preSync(float deltaTime, QHash<QQuick3DNode *, QMatrix4x4> &transformCache);
    virtual void sync();

    void markJoltObjectDirty();

    JPH::PhysicsSystem *m_jolt = nullptr;
    JPH::TempAllocator *m_tempAllocator = nullptr;
    JPH::BodyInterface *m_bodyInterface = nullptr;
    Activation m_activation = Activation::Activate;

private:
    void init(JPH::PhysicsSystem *jolt, JPH::TempAllocator *tempAllocator);
    void rebuildJoltObjectIfDirty();

    bool m_joltObjectDirty = false;
    bool m_removed = false;

    friend class PhysicsSystem;
};

#endif // ABSTRACTPHYSICSNODE_P_H
