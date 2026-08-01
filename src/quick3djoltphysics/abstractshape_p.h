#ifndef ABSTRACTSHAPE_P_H
#define ABSTRACTSHAPE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractShape : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(float density READ density WRITE setDensity NOTIFY densityChanged)
    QML_NAMED_ELEMENT(Shape)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractShape(QQuick3DNode *parent = nullptr);
    virtual ~AbstractShape();

    float density() const;
    void setDensity(float density);

    virtual JPH::Ref<JPH::Shape> getJoltShape();

signals:
    void changed();
    void densityChanged(float density);

protected:
    virtual void createJoltShape() = 0;
    void updateJoltShape();
    virtual void handleShapeChange();
    void componentComplete() override;

    QVector3D physicsScale() const;

    JPH::Ref<JPH::Shape> m_joltShape = nullptr;

private:
    void handleSceneScaleChange();
    void updateJoltShapeDensity();
    bool isCompoundChild() const;

    float m_density = 1000.0f;
    QVector3D m_prevPhysicsScale;
    QMetaObject::Connection m_materialConnection;

    friend class Body;
};

#endif // ABSTRACTSHAPE_P_H
