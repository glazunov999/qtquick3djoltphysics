#include "taperedcylindershape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCylinderShape.h>

TaperedCylinderShape::TaperedCylinderShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

TaperedCylinderShape::~TaperedCylinderShape() = default;

float TaperedCylinderShape::height() const
{
    return m_height;
}

void TaperedCylinderShape::setHeight(float height)
{
    if (qFuzzyCompare(m_height, height))
        return;

    m_height = height;
    updateJoltShape();

    emit heightChanged(m_height);
    emit changed();
}

float TaperedCylinderShape::topDiameter() const
{
    return m_topDiameter;
}

void TaperedCylinderShape::setTopDiameter(float topDiameter)
{
    if (qFuzzyCompare(m_topDiameter, topDiameter))
        return;

    m_topDiameter = topDiameter;
    updateJoltShape();

    emit topDiameterChanged(m_topDiameter);
    emit changed();
}

float TaperedCylinderShape::bottomDiameter() const
{
    return m_bottomDiameter;
}

void TaperedCylinderShape::setBottomDiameter(float bottomDiameter)
{
    if (qFuzzyCompare(m_bottomDiameter, bottomDiameter))
        return;

    m_bottomDiameter = bottomDiameter;
    updateJoltShape();

    emit bottomDiameterChanged(m_bottomDiameter);
    emit changed();
}

float TaperedCylinderShape::convexRadius() const
{
    return m_convexRadius;
}

void TaperedCylinderShape::setConvexRadius(float convexRadius)
{
    if (qFuzzyCompare(m_convexRadius, convexRadius))
        return;

    m_convexRadius = convexRadius;
    updateJoltShape();

    emit convexRadiusChanged(m_convexRadius);
    emit changed();
}

void TaperedCylinderShape::createJoltShape()
{
    JPH::TaperedCylinderShapeSettings settings(m_height * 0.5f,
                                                 m_topDiameter * 0.5f,
                                                 m_bottomDiameter * 0.5f,
                                                 m_convexRadius);
    const JPH::ShapeSettings::ShapeResult result = settings.Create();
    if (result.HasError()) {
        qWarning() << "TaperedCylinderShape: failed to create shape:" << result.GetError().c_str();
        return;
    }

    m_joltShape = result.Get();
    m_joltShape = new JPH::ScaledShape(m_joltShape, PhysicsUtils::toJoltType(physicsScale()));
}
