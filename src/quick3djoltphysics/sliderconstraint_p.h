#ifndef SLIDERCONSTRAINT_P_H
#define SLIDERCONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"
#include "springsettings_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/SliderConstraint.h>

#include <QVector3D>

#include <cfloat>

class Q_QUICK3DJOLTPHYSICS_EXPORT SliderConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(bool autoDetectPoint READ autoDetectPoint WRITE setAutoDetectPoint NOTIFY autoDetectPointChanged)
    Q_PROPERTY(QVector3D sliderAxis1 READ sliderAxis1 WRITE setSliderAxis1 NOTIFY sliderAxis1Changed)
    Q_PROPERTY(QVector3D sliderAxis2 READ sliderAxis2 WRITE setSliderAxis2 NOTIFY sliderAxis2Changed)
    Q_PROPERTY(QVector3D normalAxis1 READ normalAxis1 WRITE setNormalAxis1 NOTIFY normalAxis1Changed)
    Q_PROPERTY(QVector3D normalAxis2 READ normalAxis2 WRITE setNormalAxis2 NOTIFY normalAxis2Changed)
    Q_PROPERTY(float limitsMin READ limitsMin WRITE setLimitsMin NOTIFY limitsMinChanged)
    Q_PROPERTY(float limitsMax READ limitsMax WRITE setLimitsMax NOTIFY limitsMaxChanged)
    Q_PROPERTY(SpringSettings *limitsSpringSettings READ limitsSpringSettings WRITE setLimitsSpringSettings NOTIFY limitsSpringSettingsChanged)
    Q_PROPERTY(float maxFrictionForce READ maxFrictionForce WRITE setMaxFrictionForce NOTIFY maxFrictionForceChanged)
    QML_NAMED_ELEMENT(SliderConstraint)
public:
    explicit SliderConstraint(QQuick3DNode *parent = nullptr);
    ~SliderConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

    bool autoDetectPoint() const;
    void setAutoDetectPoint(bool autoDetectPoint);

    QVector3D sliderAxis1() const;
    void setSliderAxis1(const QVector3D &sliderAxis1);

    QVector3D sliderAxis2() const;
    void setSliderAxis2(const QVector3D &sliderAxis2);

    QVector3D normalAxis1() const;
    void setNormalAxis1(const QVector3D &normalAxis1);

    QVector3D normalAxis2() const;
    void setNormalAxis2(const QVector3D &normalAxis2);

    float limitsMin() const;
    void setLimitsMin(float limitsMin);

    float limitsMax() const;
    void setLimitsMax(float limitsMax);

    SpringSettings *limitsSpringSettings() const;
    void setLimitsSpringSettings(SpringSettings *limitsSpringSettings);

    float maxFrictionForce() const;
    void setMaxFrictionForce(float maxFrictionForce);

    Q_INVOKABLE float getCurrentPosition() const;

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);
    void autoDetectPointChanged(bool autoDetectPoint);
    void sliderAxis1Changed(const QVector3D &sliderAxis1);
    void sliderAxis2Changed(const QVector3D &sliderAxis2);
    void normalAxis1Changed(const QVector3D &normalAxis1);
    void normalAxis2Changed(const QVector3D &normalAxis2);
    void limitsMinChanged(float limitsMin);
    void limitsMaxChanged(float limitsMax);
    void limitsSpringSettingsChanged(SpringSettings *limitsSpringSettings);
    void maxFrictionForceChanged(float maxFrictionForce);

protected:
    void updateJoltObject() override;

private:
    friend class RackAndPinionConstraint;

    QVector3D m_point1;
    QVector3D m_point2;
    bool m_autoDetectPoint = true;
    QVector3D m_sliderAxis1 = QVector3D(1, 0, 0);
    QVector3D m_sliderAxis2 = QVector3D(1, 0, 0);
    QVector3D m_normalAxis1 = QVector3D(0, 1, 0);
    QVector3D m_normalAxis2 = QVector3D(0, 1, 0);
    float m_limitsMin = -FLT_MAX;
    float m_limitsMax = FLT_MAX;
    SpringSettings *m_limitsSpringSettings = nullptr;
    QMetaObject::Connection m_limitsSpringSettingsConnection;
    float m_maxFrictionForce = 0.0f;
    JPH::SliderConstraintSettings m_constraintSettings;
};

#endif // SLIDERCONSTRAINT_P_H
