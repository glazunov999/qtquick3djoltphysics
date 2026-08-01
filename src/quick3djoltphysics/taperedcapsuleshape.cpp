#include "taperedcapsuleshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCapsuleShape.h>

TaperedCapsuleShape::TaperedCapsuleShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

TaperedCapsuleShape::~TaperedCapsuleShape() = default;

float TaperedCapsuleShape::height() const
{
    return m_height;
}

void TaperedCapsuleShape::setHeight(float height)
{
    if (qFuzzyCompare(m_height, height))
        return;

    m_height = height;
    updateJoltShape();

    emit heightChanged(m_height);
    emit changed();
}

float TaperedCapsuleShape::topDiameter() const
{
    return m_topDiameter;
}

void TaperedCapsuleShape::setTopDiameter(float topDiameter)
{
    if (qFuzzyCompare(m_topDiameter, topDiameter))
        return;

    m_topDiameter = topDiameter;
    updateJoltShape();

    emit topDiameterChanged(m_topDiameter);
    emit changed();
}

float TaperedCapsuleShape::bottomDiameter() const
{
    return m_bottomDiameter;
}

void TaperedCapsuleShape::setBottomDiameter(float bottomDiameter)
{
    if (qFuzzyCompare(m_bottomDiameter, bottomDiameter))
        return;

    m_bottomDiameter = bottomDiameter;
    updateJoltShape();

    emit bottomDiameterChanged(m_bottomDiameter);
    emit changed();
}

void TaperedCapsuleShape::createJoltShape()
{
    JPH::TaperedCapsuleShapeSettings settings(m_height * 0.5f,
                                              m_topDiameter * 0.5f,
                                              m_bottomDiameter * 0.5f);
    const JPH::ShapeSettings::ShapeResult result = settings.Create();
    if (result.HasError()) {
        qWarning() << "TaperedCapsuleShape: failed to create shape:" << result.GetError().c_str();
        return;
    }

    m_joltShape = result.Get();
    m_joltShape = new JPH::ScaledShape(m_joltShape, PhysicsUtils::toJoltType(physicsScale()));
}
