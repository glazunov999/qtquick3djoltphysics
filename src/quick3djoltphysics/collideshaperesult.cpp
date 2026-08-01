#include "collideshaperesult_p.h"

CollideShapeResult::CollideShapeResult() = default;

CollideShapeResult::CollideShapeResult(const QVector3D &contactPointOn1,
                                       const QVector3D &contactPointOn2,
                                       const QVector3D &penetrationAxis,
                                       float penetrationDepth,
                                       const QVector3D &surfaceNormal,
                                       AbstractPhysicsBody *body2,
                                       SoftBody *softBody2)
    : m_contactPointOn1(contactPointOn1)
    , m_contactPointOn2(contactPointOn2)
    , m_penetrationAxis(penetrationAxis)
    , m_penetrationDepth(penetrationDepth)
    , m_surfaceNormal(surfaceNormal)
    , m_body2(body2)
    , m_softBody2(softBody2)
{
}

QVector3D CollideShapeResult::contactPointOn1() const
{
    return m_contactPointOn1;
}

QVector3D CollideShapeResult::contactPointOn2() const
{
    return m_contactPointOn2;
}

QVector3D CollideShapeResult::penetrationAxis() const
{
    return m_penetrationAxis;
}

float CollideShapeResult::penetrationDepth() const
{
    return m_penetrationDepth;
}

QVector3D CollideShapeResult::surfaceNormal() const
{
    return m_surfaceNormal;
}

AbstractPhysicsBody *CollideShapeResult::body2() const
{
    return m_body2;
}

SoftBody *CollideShapeResult::softBody2() const
{
    return m_softBody2;
}
