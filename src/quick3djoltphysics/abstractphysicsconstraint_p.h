#ifndef ABSTRACTCONSTRAINT_P_H
#define ABSTRACTCONSTRAINT_P_H

#include "abstractphysicsnode_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>

#include <QtQml/QQmlEngine>
#include <QObject>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Constraints/Constraint.h>

namespace JPH {
class Constraint;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsConstraintSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(quint32 constraintPriority READ constraintPriority WRITE setConstraintPriority NOTIFY constraintPriorityChanged)
    Q_PROPERTY(uint numVelocityStepsOverride READ numVelocityStepsOverride WRITE setNumVelocityStepsOverride NOTIFY numVelocityStepsOverrideChanged)
    Q_PROPERTY(uint numPositionStepsOverride READ numPositionStepsOverride WRITE setNumPositionStepsOverride NOTIFY numPositionStepsOverrideChanged)
    Q_PROPERTY(float drawConstraintSize READ drawConstraintSize WRITE setDrawConstraintSize NOTIFY drawConstraintSizeChanged)
    QML_NAMED_ELEMENT(PhysicsConstraintSettings)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsConstraintSettings(QObject *parent = nullptr);
    ~AbstractPhysicsConstraintSettings() override;

    bool enabled() const;
    void setEnabled(bool enabled);

    quint32 constraintPriority() const;
    void setConstraintPriority(quint32 constraintPriority);

    uint numVelocityStepsOverride() const;
    void setNumVelocityStepsOverride(uint numVelocityStepsOverride);

    uint numPositionStepsOverride() const;
    void setNumPositionStepsOverride(uint numPositionStepsOverride);

    float drawConstraintSize() const;
    void setDrawConstraintSize(float drawConstraintSize);

    virtual JPH::Ref<JPH::ConstraintSettings> createJoltConstraintSettings() const = 0;

signals:
    void enabledChanged(bool enabled);
    void constraintPriorityChanged(quint32 constraintPriority);
    void numVelocityStepsOverrideChanged(uint numVelocityStepsOverride);
    void numPositionStepsOverrideChanged(uint numPositionStepsOverride);
    void drawConstraintSizeChanged(float drawConstraintSize);
    void changed();

protected:
    void applyBaseSettings(JPH::ConstraintSettings &settings) const;

    bool m_enabled = true;
    quint32 m_constraintPriority = 0;
    uint m_numVelocityStepsOverride = 0;
    uint m_numPositionStepsOverride = 0;
    float m_drawConstraintSize = 1.0f;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsConstraint : public AbstractPhysicsNode
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhysicsConstraint)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsConstraint(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsConstraint() override;

protected:
    void cleanup() override;

    JPH::Constraint *m_constraint = nullptr;
};

#endif // ABSTRACTCONSTRAINT_P_H
