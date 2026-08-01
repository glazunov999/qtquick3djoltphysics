#include "abstractshape_p.h"
#include "abstractcompoundshape_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D>

#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/DecoratedShape.h>

AbstractShape::AbstractShape(QQuick3DNode *parent) : QQuick3DNode(parent)
{
    connect(this, &QQuick3DNode::sceneScaleChanged, this, &AbstractShape::handleSceneScaleChange);
    connect(this, &QQuick3DNode::positionChanged, this, [this] {
        if (isCompoundChild())
            emit changed();
        else
            handleShapeChange();
    });
    connect(this, &QQuick3DNode::rotationChanged, this, [this] {
        if (isCompoundChild())
            emit changed();
        else
            handleShapeChange();
    });
}

AbstractShape::~AbstractShape() = default;

bool AbstractShape::isCompoundChild() const
{
    return qobject_cast<const AbstractCompoundShape *>(parentNode()) != nullptr;
}

QVector3D AbstractShape::physicsScale() const
{
    if (isCompoundChild())
        return QVector3D(1, 1, 1);

    QVector3D result = scale();
    for (const QQuick3DNode *node = parentNode(); node != nullptr; node = node->parentNode()) {
        const QVector3D parentScale = node->scale();
        result = QVector3D(result.x() * parentScale.x(),
                           result.y() * parentScale.y(),
                           result.z() * parentScale.z());
    }
    return result;
}

float AbstractShape::density() const
{
    return m_density;
}

void AbstractShape::setDensity(float density)
{
    if (qFuzzyCompare(m_density, density))
        return;

    m_density = density;
    updateJoltShapeDensity();

    emit densityChanged(m_density);
}

JPH::Ref<JPH::Shape> AbstractShape::getJoltShape()
{
    if (m_joltShape == nullptr)
        updateJoltShape();

    return m_joltShape;
}

void AbstractShape::updateJoltShape()
{
    m_joltShape = nullptr;
    
    createJoltShape();

    updateJoltShapeDensity();
}

void AbstractShape::handleSceneScaleChange()
{
    if (isCompoundChild())
        return;

    const QVector3D newScale = physicsScale();
    if (qFuzzyCompare(newScale, m_prevPhysicsScale))
        return;

    m_prevPhysicsScale = newScale;
    handleShapeChange();
}

void AbstractShape::handleShapeChange()
{
    if (!isComponentComplete())
        return;

    updateJoltShape();
    emit changed();
}

void AbstractShape::componentComplete()
{
    QQuick3DNode::componentComplete();
    m_prevPhysicsScale = physicsScale();
}

static JPH::ConvexShape *getInnerConvexShape(JPH::Shape *shape)
{
    if (!shape)
        return nullptr;

    while (shape->GetType() == JPH::EShapeType::Decorated) {
        auto *decorated = static_cast<JPH::DecoratedShape *>(shape);
        shape = const_cast<JPH::Shape *>(decorated->GetInnerShape());
    }

    if (shape->GetType() == JPH::EShapeType::Convex)
        return static_cast<JPH::ConvexShape *>(shape);

    return nullptr;
}

void AbstractShape::updateJoltShapeDensity()
{
    if (!m_joltShape)
        return;

    auto *convexShape = getInnerConvexShape(m_joltShape.GetPtr());
    if (convexShape)
        convexShape->SetDensity(m_density);
    else if (m_joltShape)
        qWarning() << "AbstractShape::updateJoltShapeDensity: Cannot set density on non-convex shape";
}
