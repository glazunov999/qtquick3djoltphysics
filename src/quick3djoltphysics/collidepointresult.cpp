#include "collidepointresult_p.h"

CollidePointResult::CollidePointResult() = default;

CollidePointResult::CollidePointResult(AbstractPhysicsBody *body, SoftBody *softBody)
    : m_body(body)
    , m_softBody(softBody)
{
}

AbstractPhysicsBody *CollidePointResult::body() const
{
    return m_body;
}

SoftBody *CollidePointResult::softBody() const
{
    return m_softBody;
}
