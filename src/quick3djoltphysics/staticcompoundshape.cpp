#include "staticcompoundshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>

StaticCompoundShape::StaticCompoundShape(QQuick3DNode *parent) : AbstractCompoundShape(parent)
{
}

StaticCompoundShape::~StaticCompoundShape() = default;

void StaticCompoundShape::createJoltShape()
{
    if (!isComponentComplete() || m_shapes.isEmpty())
        return;

    JPH::StaticCompoundShapeSettings staticCompoundSettings;
    for (auto *shape : std::as_const(m_shapes)) {
        staticCompoundSettings.AddShape(PhysicsUtils::toJoltType(shape->position()),
                                        PhysicsUtils::toJoltType(shape->rotation()),
                                        shape->getJoltShape());
    }

    const JPH::ShapeSettings::ShapeResult shapeResult = staticCompoundSettings.Create();
    if (shapeResult.HasError()) {
        qWarning() << "StaticCompoundShape: failed to create shape:" << shapeResult.GetError().c_str();
        return;
    }

    m_joltShape = new JPH::ScaledShape(shapeResult.Get(), PhysicsUtils::toJoltType(physicsScale()));
}
