#ifndef CONECONSTRAINT_P_H
#define CONECONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/ConeConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT ConeConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(QVector3D twistAxis1 READ twistAxis1 WRITE setTwistAxis1 NOTIFY twistAxis1Changed)
    Q_PROPERTY(QVector3D twistAxis2 READ twistAxis2 WRITE setTwistAxis2 NOTIFY twistAxis2Changed)
    Q_PROPERTY(float halfConeAngle READ halfConeAngle WRITE setHalfConeAngle NOTIFY halfConeAngleChanged)
    QML_NAMED_ELEMENT(ConeConstraint)
public:
    explicit ConeConstraint(QQuick3DNode *parent = nullptr);
    ~ConeConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

    QVector3D twistAxis1() const;
    void setTwistAxis1(const QVector3D &twistAxis1);

    QVector3D twistAxis2() const;
    void setTwistAxis2(const QVector3D &twistAxis2);

    float halfConeAngle() const;
    void setHalfConeAngle(float halfConeAngle);

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);
    void twistAxis1Changed(const QVector3D &twistAxis1);
    void twistAxis2Changed(const QVector3D &twistAxis2);
    void halfConeAngleChanged(float halfConeAngle);

protected:
    void updateJoltObject() override;

private:
    QVector3D m_point1;
    QVector3D m_point2;
    QVector3D m_twistAxis1 = QVector3D(1, 0, 0);
    QVector3D m_twistAxis2 = QVector3D(1, 0, 0);
    float m_halfConeAngle = 0.0f;
    JPH::ConeConstraintSettings m_constraintSettings;
};

#endif // CONECONSTRAINT_P_H
