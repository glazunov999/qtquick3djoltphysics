#ifndef PULLEYCONSTRAINT_P_H
#define PULLEYCONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/PulleyConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT PulleyConstraintSettings : public AbstractTwoBodyPhysicsConstraintSettings
{
    Q_OBJECT
    Q_PROPERTY(QVector3D bodyPoint1 READ bodyPoint1 WRITE setBodyPoint1 NOTIFY bodyPoint1Changed)
    Q_PROPERTY(QVector3D fixedPoint1 READ fixedPoint1 WRITE setFixedPoint1 NOTIFY fixedPoint1Changed)
    Q_PROPERTY(QVector3D bodyPoint2 READ bodyPoint2 WRITE setBodyPoint2 NOTIFY bodyPoint2Changed)
    Q_PROPERTY(QVector3D fixedPoint2 READ fixedPoint2 WRITE setFixedPoint2 NOTIFY fixedPoint2Changed)
    Q_PROPERTY(float ratio READ ratio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY(float minLength READ minLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(float maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    QML_NAMED_ELEMENT(PulleyConstraintSettings)
public:
    explicit PulleyConstraintSettings(QObject *parent = nullptr);

    QVector3D bodyPoint1() const;
    void setBodyPoint1(const QVector3D &bodyPoint1);

    QVector3D fixedPoint1() const;
    void setFixedPoint1(const QVector3D &fixedPoint1);

    QVector3D bodyPoint2() const;
    void setBodyPoint2(const QVector3D &bodyPoint2);

    QVector3D fixedPoint2() const;
    void setFixedPoint2(const QVector3D &fixedPoint2);

    float ratio() const;
    void setRatio(float ratio);

    float minLength() const;
    void setMinLength(float minLength);

    float maxLength() const;
    void setMaxLength(float maxLength);

    JPH::Ref<JPH::TwoBodyConstraintSettings> createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame = nullptr) const override;
    void mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                    const QQuick3DNode *localFrame) const override;

signals:
    void bodyPoint1Changed(const QVector3D &bodyPoint1);
    void fixedPoint1Changed(const QVector3D &fixedPoint1);
    void bodyPoint2Changed(const QVector3D &bodyPoint2);
    void fixedPoint2Changed(const QVector3D &fixedPoint2);
    void ratioChanged(float ratio);
    void minLengthChanged(float minLength);
    void maxLengthChanged(float maxLength);

private:
    QVector3D m_bodyPoint1;
    QVector3D m_fixedPoint1;
    QVector3D m_bodyPoint2;
    QVector3D m_fixedPoint2;
    float m_ratio = 1.0f;
    float m_minLength = 0.0f;
    float m_maxLength = -1.0f;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT PulleyConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(PulleyConstraintSettings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    QML_NAMED_ELEMENT(PulleyConstraint)
public:
    explicit PulleyConstraint(QQuick3DNode *parent = nullptr);
    ~PulleyConstraint() override;

    PulleyConstraintSettings *settings() const;
    void setSettings(PulleyConstraintSettings *settings);

    Q_INVOKABLE float getCurrentLength() const;
    Q_INVOKABLE float getTotalLambdaPosition() const;

signals:
    void settingsChanged(PulleyConstraintSettings *settings);

protected:
    void updateJoltObject() override;

private:
    PulleyConstraintSettings *m_settings = nullptr;
};

#endif // PULLEYCONSTRAINT_P_H
