#include "raycastresult_p.h"

RayCastResult::RayCastResult() = default;

RayCastResult::RayCastResult(AbstractPhysicsBody *body, SoftBody *softBody, const QVector3D &position, const QVector3D &normal, float fraction)
    : m_body(body)
    , m_softBody(softBody)
    , m_position(position)
    , m_normal(normal)
    , m_fraction(fraction)
{
}

AbstractPhysicsBody *RayCastResult::body() const
{
    return m_body;
}

SoftBody *RayCastResult::softBody() const
{
    return m_softBody;
}

QVector3D RayCastResult::position() const
{
    return m_position;
}

QVector3D RayCastResult::normal() const
{
    return m_normal;
}

float RayCastResult::fraction() const
{
    return m_fraction;
}
