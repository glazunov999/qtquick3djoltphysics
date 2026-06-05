#ifndef POINTCONSTRAINT_P_H
#define POINTCONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/PointConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT PointConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    QML_NAMED_ELEMENT(PointConstraint)
public:
    explicit PointConstraint(QQuick3DNode *parent = nullptr);
    ~PointConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);

protected:
    void updateJoltObject() override;

private:
    QVector3D m_point1;
    QVector3D m_point2;
    JPH::PointConstraintSettings m_constraintSettings;
};

#endif // POINTCONSTRAINT_P_H
