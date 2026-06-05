#ifndef ABSTRACTTWOBODYPHYSICSCONSTRAINT_P_H
#define ABSTRACTTWOBODYPHYSICSCONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

namespace JPH {
class Body;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractTwoBodyPhysicsConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(ConstraintSpace space READ space WRITE setSpace NOTIFY spaceChanged)
    QML_NAMED_ELEMENT(PhysicsTwoBodyConstraint)
    QML_UNCREATABLE("abstract interface")
public:
    enum class ConstraintSpace {
        LocalToBodyCOM = 0,
        WorldSpace = 1,
    };
    Q_ENUM(ConstraintSpace)

    explicit AbstractTwoBodyPhysicsConstraint(QQuick3DNode *parent = nullptr);
    ~AbstractTwoBodyPhysicsConstraint() override;

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    ConstraintSpace space() const;
    void setSpace(ConstraintSpace space);

signals:
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void spaceChanged(ConstraintSpace space);

protected:
    bool joltBodiesReady() const;
    JPH::Body *joltBody1() const;
    JPH::Body *joltBody2() const;

    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    ConstraintSpace m_space = ConstraintSpace::WorldSpace;

private:
    QMetaObject::Connection m_body1SignalConnection;
    QMetaObject::Connection m_body2SignalConnection;
};

#endif // ABSTRACTTWOBODYPHYSICSCONSTRAINT_P_H
