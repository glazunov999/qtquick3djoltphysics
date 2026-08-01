#include "abstractcompoundshape_p.h"

AbstractCompoundShape::AbstractCompoundShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

AbstractCompoundShape::~AbstractCompoundShape() = default;

void AbstractCompoundShape::componentComplete()
{
    AbstractShape::componentComplete();
    handleShapeChange();
}

void AbstractCompoundShape::handleShapeChange()
{
    if (!isComponentComplete())
        return;

    AbstractShape::handleShapeChange();
}

QQmlListProperty<AbstractShape> AbstractCompoundShape::shapes()
{
    return QQmlListProperty<AbstractShape>(
        this, nullptr, AbstractCompoundShape::qmlAppendShape,
        AbstractCompoundShape::qmlShapeCount, AbstractCompoundShape::qmlShapeAt,
        AbstractCompoundShape::qmlClearShapes);
}

int AbstractCompoundShape::shapeCount() const
{
    return m_shapes.size();
}

AbstractShape *AbstractCompoundShape::shapeAt(int index) const
{
    if (index < 0 || index >= m_shapes.size())
        return nullptr;
    return m_shapes.at(index);
}

void AbstractCompoundShape::appendShape(AbstractShape *shape)
{
    if (shape == nullptr)
        return;

    m_shapes.push_back(shape);
    if (shape->parentItem() == nullptr)
        shape->setParentItem(this);

    connect(shape, &QObject::destroyed, this, [this](QObject *shapeObject) {
        m_shapes.removeAll(static_cast<AbstractShape *>(shapeObject));
    });

    connect(shape, &AbstractShape::changed, this, [this] {
        handleShapeChange();
    });
}

void AbstractCompoundShape::clearShapes()
{
    for (auto *shape : std::as_const(m_shapes))
        shape->disconnect(this);

    m_shapes.clear();
}

void AbstractCompoundShape::qmlAppendShape(QQmlListProperty<AbstractShape> *list, AbstractShape *shape)
{
    auto *self = static_cast<AbstractCompoundShape *>(list->object);
    self->appendShape(shape);
    self->handleShapeChange();
}

AbstractShape *AbstractCompoundShape::qmlShapeAt(QQmlListProperty<AbstractShape> *list, qsizetype index)
{
    auto *self = static_cast<AbstractCompoundShape *>(list->object);
    return self->m_shapes.at(index);
}

qsizetype AbstractCompoundShape::qmlShapeCount(QQmlListProperty<AbstractShape> *list)
{
    auto *self = static_cast<AbstractCompoundShape *>(list->object);
    return self->m_shapes.count();
}

void AbstractCompoundShape::qmlClearShapes(QQmlListProperty<AbstractShape> *list)
{
    auto *self = static_cast<AbstractCompoundShape *>(list->object);
    self->clearShapes();
    self->handleShapeChange();
}
