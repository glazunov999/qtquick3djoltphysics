#ifndef GEARCONSTRAINT_P_H
#define GEARCONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"
#include "hingeconstraint_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/GearConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT GearConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D hingeAxis1 READ hingeAxis1 WRITE setHingeAxis1 NOTIFY hingeAxis1Changed)
    Q_PROPERTY(QVector3D hingeAxis2 READ hingeAxis2 WRITE setHingeAxis2 NOTIFY hingeAxis2Changed)
    Q_PROPERTY(float ratio READ ratio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(HingeConstraint *gear1Constraint READ gear1Constraint WRITE setGear1Constraint NOTIFY gear1ConstraintChanged)
    Q_PROPERTY(HingeConstraint *gear2Constraint READ gear2Constraint WRITE setGear2Constraint NOTIFY gear2ConstraintChanged)
    QML_NAMED_ELEMENT(GearConstraint)
public:
    explicit GearConstraint(QQuick3DNode *parent = nullptr);
    ~GearConstraint() override;

    QVector3D hingeAxis1() const;
    void setHingeAxis1(const QVector3D &hingeAxis1);

    QVector3D hingeAxis2() const;
    void setHingeAxis2(const QVector3D &hingeAxis2);

    float ratio() const;
    void setRatio(float ratio);

    HingeConstraint *gear1Constraint() const;
    void setGear1Constraint(HingeConstraint *constraint);

    HingeConstraint *gear2Constraint() const;
    void setGear2Constraint(HingeConstraint *constraint);

    Q_INVOKABLE float getTotalLambda() const;

signals:
    void hingeAxis1Changed(const QVector3D &hingeAxis1);
    void hingeAxis2Changed(const QVector3D &hingeAxis2);
    void ratioChanged(float ratio);
    void gear1ConstraintChanged(HingeConstraint *constraint);
    void gear2ConstraintChanged(HingeConstraint *constraint);

protected:
    void updateJoltObject() override;

private:
    void linkGearConstraints();
    void watchGearHinge(HingeConstraint *hinge, QMetaObject::Connection &body1Connection,
                        QMetaObject::Connection &body2Connection);

    QVector3D m_hingeAxis1 = QVector3D(1, 0, 0);
    QVector3D m_hingeAxis2 = QVector3D(1, 0, 0);
    float m_ratio = 1.0f;
    HingeConstraint *m_gear1Constraint = nullptr;
    HingeConstraint *m_gear2Constraint = nullptr;
    QMetaObject::Connection m_gear1Body1SignalConnection;
    QMetaObject::Connection m_gear1Body2SignalConnection;
    QMetaObject::Connection m_gear2Body1SignalConnection;
    QMetaObject::Connection m_gear2Body2SignalConnection;
    JPH::GearConstraintSettings m_constraintSettings;
};

#endif // GEARCONSTRAINT_P_H
