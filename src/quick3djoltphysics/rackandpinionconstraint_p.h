#ifndef RACKANDPINIONCONSTRAINT_P_H
#define RACKANDPINIONCONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"
#include "hingeconstraint_p.h"
#include "sliderconstraint_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/RackAndPinionConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT RackAndPinionConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(QVector3D hingeAxis READ hingeAxis WRITE setHingeAxis NOTIFY hingeAxisChanged)
    Q_PROPERTY(QVector3D sliderAxis READ sliderAxis WRITE setSliderAxis NOTIFY sliderAxisChanged)
    Q_PROPERTY(float ratio READ ratio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(HingeConstraint *pinionConstraint READ pinionConstraint WRITE setPinionConstraint NOTIFY pinionConstraintChanged)
    Q_PROPERTY(SliderConstraint *rackConstraint READ rackConstraint WRITE setRackConstraint NOTIFY rackConstraintChanged)
    QML_NAMED_ELEMENT(RackAndPinionConstraint)
public:
    explicit RackAndPinionConstraint(QQuick3DNode *parent = nullptr);
    ~RackAndPinionConstraint() override;

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    QVector3D hingeAxis() const;
    void setHingeAxis(const QVector3D &hingeAxis);

    QVector3D sliderAxis() const;
    void setSliderAxis(const QVector3D &sliderAxis);

    float ratio() const;
    void setRatio(float ratio);

    HingeConstraint *pinionConstraint() const;
    void setPinionConstraint(HingeConstraint *constraint);

    SliderConstraint *rackConstraint() const;
    void setRackConstraint(SliderConstraint *constraint);

    Q_INVOKABLE float getTotalLambda() const;

    Q_INVOKABLE static float ratioFromTeeth(int rackNumTeeth, float rackLength, int pinionNumTeeth);

signals:
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void hingeAxisChanged(const QVector3D &hingeAxis);
    void sliderAxisChanged(const QVector3D &sliderAxis);
    void ratioChanged(float ratio);
    void pinionConstraintChanged(HingeConstraint *constraint);
    void rackConstraintChanged(SliderConstraint *constraint);

protected:
    void updateJoltObject() override;

private:
    void linkRackAndPinionConstraints();
    void watchPinionHinge(HingeConstraint *hinge, QMetaObject::Connection &body1Connection,
                          QMetaObject::Connection &body2Connection);
    void watchRackSlider(SliderConstraint *slider, QMetaObject::Connection &body1Connection,
                         QMetaObject::Connection &body2Connection);

    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    QVector3D m_hingeAxis = QVector3D(0, 0, 1);
    QVector3D m_sliderAxis = QVector3D(1, 0, 0);
    float m_ratio = 1.0f;
    HingeConstraint *m_pinionConstraint = nullptr;
    SliderConstraint *m_rackConstraint = nullptr;
    QMetaObject::Connection m_body1SignalConnection;
    QMetaObject::Connection m_body2SignalConnection;
    QMetaObject::Connection m_pinionBody1SignalConnection;
    QMetaObject::Connection m_pinionBody2SignalConnection;
    QMetaObject::Connection m_rackBody1SignalConnection;
    QMetaObject::Connection m_rackBody2SignalConnection;
    JPH::RackAndPinionConstraintSettings m_constraintSettings;
};

#endif // RACKANDPINIONCONSTRAINT_P_H
