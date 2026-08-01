#ifndef COLLIDEPOINTRESULT_P_H
#define COLLIDEPOINTRESULT_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include "abstractphysicsbody_p.h"
#include "softbody_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT CollidePointResult
{
    Q_GADGET
    Q_PROPERTY(AbstractPhysicsBody *body READ body CONSTANT)
    Q_PROPERTY(SoftBody *softBody READ softBody CONSTANT)
public:
    CollidePointResult();
    CollidePointResult(AbstractPhysicsBody *body, SoftBody *softBody);

    AbstractPhysicsBody *body() const;
    SoftBody *softBody() const;

private:
    AbstractPhysicsBody *m_body = nullptr;
    SoftBody *m_softBody = nullptr;
};

Q_DECLARE_METATYPE(CollidePointResult)

#endif // COLLIDEPOINTRESULT_P_H
