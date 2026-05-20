#ifndef PULLEYCONSTRAINT_P_H
#define PULLEYCONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/PulleyConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT PulleyConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(QVector3D bodyPoint1 READ bodyPoint1 WRITE setBodyPoint1 NOTIFY bodyPoint1Changed)
    Q_PROPERTY(QVector3D fixedPoint1 READ fixedPoint1 WRITE setFixedPoint1 NOTIFY fixedPoint1Changed)
    Q_PROPERTY(QVector3D bodyPoint2 READ bodyPoint2 WRITE setBodyPoint2 NOTIFY bodyPoint2Changed)
    Q_PROPERTY(QVector3D fixedPoint2 READ fixedPoint2 WRITE setFixedPoint2 NOTIFY fixedPoint2Changed)
    Q_PROPERTY(float ratio READ ratio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(float minLength READ minLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(float maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    QML_NAMED_ELEMENT(PulleyConstraint)
public:
    explicit PulleyConstraint(QQuick3DNode *parent = nullptr);
    ~PulleyConstraint() override;

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    QVector3D bodyPoint1() const;
    void setBodyPoint1(const QVector3D &point);

    QVector3D fixedPoint1() const;
    void setFixedPoint1(const QVector3D &point);

    QVector3D bodyPoint2() const;
    void setBodyPoint2(const QVector3D &point);

    QVector3D fixedPoint2() const;
    void setFixedPoint2(const QVector3D &point);

    float ratio() const;
    void setRatio(float ratio);

    float minLength() const;
    void setMinLength(float minLength);

    float maxLength() const;
    void setMaxLength(float maxLength);

    Q_INVOKABLE float getCurrentLength() const;
    Q_INVOKABLE float getTotalLambdaPosition() const;

signals:
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void bodyPoint1Changed(const QVector3D &point);
    void fixedPoint1Changed(const QVector3D &point);
    void bodyPoint2Changed(const QVector3D &point);
    void fixedPoint2Changed(const QVector3D &point);
    void ratioChanged(float ratio);
    void minLengthChanged(float minLength);
    void maxLengthChanged(float maxLength);

protected:
    void updateJoltObject() override;

private:
    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    QVector3D m_bodyPoint1;
    QVector3D m_fixedPoint1;
    QVector3D m_bodyPoint2;
    QVector3D m_fixedPoint2;
    float m_ratio = 1.0f;
    float m_minLength = 0.0f;
    float m_maxLength = -1.0f;
    QMetaObject::Connection m_body1SignalConnection;
    QMetaObject::Connection m_body2SignalConnection;
    JPH::PulleyConstraintSettings m_constraintSettings;
};

#endif // PULLEYCONSTRAINT_P_H
