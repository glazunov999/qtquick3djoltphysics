#ifndef COLLIDESHAPERESULT_P_H
#define COLLIDESHAPERESULT_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QVector3D>

#include "abstractphysicsbody_p.h"
#include "softbody_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT CollideShapeResult
{
    Q_GADGET
    Q_PROPERTY(QVector3D contactPointOn1 READ contactPointOn1 CONSTANT)
    Q_PROPERTY(QVector3D contactPointOn2 READ contactPointOn2 CONSTANT)
    Q_PROPERTY(QVector3D penetrationAxis READ penetrationAxis CONSTANT)
    Q_PROPERTY(float penetrationDepth READ penetrationDepth CONSTANT)
    Q_PROPERTY(QVector3D surfaceNormal READ surfaceNormal CONSTANT)
    Q_PROPERTY(AbstractPhysicsBody *body2 READ body2 CONSTANT)
    Q_PROPERTY(SoftBody *softBody2 READ softBody2 CONSTANT)
public:
    CollideShapeResult();
    CollideShapeResult(const QVector3D &contactPointOn1,
                       const QVector3D &contactPointOn2,
                       const QVector3D &penetrationAxis,
                       float penetrationDepth,
                       const QVector3D &surfaceNormal,
                       AbstractPhysicsBody *body2,
                       SoftBody *softBody2);

    QVector3D contactPointOn1() const;
    QVector3D contactPointOn2() const;
    QVector3D penetrationAxis() const;
    float penetrationDepth() const;
    QVector3D surfaceNormal() const;
    AbstractPhysicsBody *body2() const;
    SoftBody *softBody2() const;

private:
    QVector3D m_contactPointOn1;
    QVector3D m_contactPointOn2;
    QVector3D m_penetrationAxis;
    float m_penetrationDepth = 0.0f;
    QVector3D m_surfaceNormal;
    AbstractPhysicsBody *m_body2 = nullptr;
    SoftBody *m_softBody2 = nullptr;
};

#endif // COLLIDESHAPERESULT_P_H
