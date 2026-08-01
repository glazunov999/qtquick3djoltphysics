#ifndef ABSTRACTCOMPOUNDSHAPE_P_H
#define ABSTRACTCOMPOUNDSHAPE_P_H

#include "abstractshape_p.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>

#include <QVector>

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractCompoundShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractShape> shapes READ shapes CONSTANT)
    QML_NAMED_ELEMENT(CompoundShape)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractCompoundShape(QQuick3DNode *parent = nullptr);
    ~AbstractCompoundShape() override;

    QQmlListProperty<AbstractShape> shapes();

    Q_INVOKABLE int shapeCount() const;
    Q_INVOKABLE AbstractShape *shapeAt(int index) const;

protected:
    void componentComplete() override;
    void handleShapeChange() override;

    void appendShape(AbstractShape *shape);
    void clearShapes();

    QVector<AbstractShape *> m_shapes;

private:
    static void qmlAppendShape(QQmlListProperty<AbstractShape> *list, AbstractShape *shape);
    static AbstractShape *qmlShapeAt(QQmlListProperty<AbstractShape> *list, qsizetype index);
    static qsizetype qmlShapeCount(QQmlListProperty<AbstractShape> *list);
    static void qmlClearShapes(QQmlListProperty<AbstractShape> *list);
};

#endif // ABSTRACTCOMPOUNDSHAPE_P_H
