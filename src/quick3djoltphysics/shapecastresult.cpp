#include "shapecastresult_p.h"

ShapeCastResult::ShapeCastResult() = default;

ShapeCastResult::ShapeCastResult(const QVector3D &contactPointOn1,
                                 const QVector3D &contactPointOn2,
                                 const QVector3D &penetrationAxis,
                                 float penetrationDepth,
                                 const QVector3D &surfaceNormal,
                                 AbstractPhysicsBody *body2,
                                 SoftBody *softBody2,
                                 const QVector3D &position,
                                 float fraction,
                                 bool isBackFaceHit)
    : m_contactPointOn1(contactPointOn1)
    , m_contactPointOn2(contactPointOn2)
    , m_penetrationAxis(penetrationAxis)
    , m_penetrationDepth(penetrationDepth)
    , m_surfaceNormal(surfaceNormal)
    , m_body2(body2)
    , m_softBody2(softBody2)
    , m_position(position)
    , m_fraction(fraction)
    , m_isBackFaceHit(isBackFaceHit)
{
}

QVector3D ShapeCastResult::contactPointOn1() const
{
    return m_contactPointOn1;
}

QVector3D ShapeCastResult::contactPointOn2() const
{
    return m_contactPointOn2;
}

QVector3D ShapeCastResult::penetrationAxis() const
{
    return m_penetrationAxis;
}

float ShapeCastResult::penetrationDepth() const
{
    return m_penetrationDepth;
}

QVector3D ShapeCastResult::surfaceNormal() const
{
    return m_surfaceNormal;
}

AbstractPhysicsBody *ShapeCastResult::body2() const
{
    return m_body2;
}

SoftBody *ShapeCastResult::softBody2() const
{
    return m_softBody2;
}

QVector3D ShapeCastResult::position() const
{
    return m_position;
}

float ShapeCastResult::fraction() const
{
    return m_fraction;
}

bool ShapeCastResult::isBackFaceHit() const
{
    return m_isBackFaceHit;
}
