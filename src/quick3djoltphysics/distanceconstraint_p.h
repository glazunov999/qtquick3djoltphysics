#ifndef DISTANCECONSTRAINT_P_H
#define DISTANCECONSTRAINT_P_H

#include "abstracttwobodyphysicsconstraint_p.h"
#include "springsettings_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/DistanceConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT DistanceConstraintSettings : public AbstractTwoBodyPhysicsConstraintSettings
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(float minDistance READ minDistance WRITE setMinDistance NOTIFY minDistanceChanged)
    Q_PROPERTY(float maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(SpringSettings *limitsSpringSettings READ limitsSpringSettings WRITE setLimitsSpringSettings NOTIFY limitsSpringSettingsChanged)
    QML_NAMED_ELEMENT(DistanceConstraintSettings)
public:
    explicit DistanceConstraintSettings(QObject *parent = nullptr);

    QVector3D point1() const;
    void setPoint1(const QVector3D &point1);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point2);

    float minDistance() const;
    void setMinDistance(float minDistance);

    float maxDistance() const;
    void setMaxDistance(float maxDistance);

    SpringSettings *limitsSpringSettings() const;
    void setLimitsSpringSettings(SpringSettings *limitsSpringSettings);

    JPH::Ref<JPH::TwoBodyConstraintSettings> createJoltTwoBodyConstraintSettings(const QQuick3DNode *localFrame = nullptr) const override;
    void mapToWorld(JPH::TwoBodyConstraintSettings *settings,
                    const QQuick3DNode *localFrame) const override;

signals:
    void point1Changed(const QVector3D &point1);
    void point2Changed(const QVector3D &point2);
    void minDistanceChanged(float minDistance);
    void maxDistanceChanged(float maxDistance);
    void limitsSpringSettingsChanged(SpringSettings *limitsSpringSettings);

private:
    QVector3D m_point1;
    QVector3D m_point2;
    float m_minDistance = -1.0f;
    float m_maxDistance = -1.0f;
    SpringSettings *m_limitsSpringSettings = nullptr;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT DistanceConstraint : public AbstractTwoBodyPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(DistanceConstraintSettings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    QML_NAMED_ELEMENT(DistanceConstraint)
public:
    explicit DistanceConstraint(QQuick3DNode *parent = nullptr);
    ~DistanceConstraint() override;

    DistanceConstraintSettings *settings() const;
    void setSettings(DistanceConstraintSettings *settings);

signals:
    void settingsChanged(DistanceConstraintSettings *settings);

protected:
    void updateJoltObject() override;

private:
    DistanceConstraintSettings *m_settings = nullptr;
};

#endif // DISTANCECONSTRAINT_P_H
