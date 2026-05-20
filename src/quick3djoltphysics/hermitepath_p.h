#ifndef HERMITEPATH_P_H
#define HERMITEPATH_P_H

#include "pathconstraintpath_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>
#include <QVector3D>

#include <Jolt/Physics/Constraints/PathConstraintPathHermite.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT HermitePathPoint : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D tangent READ tangent WRITE setTangent NOTIFY tangentChanged)
    Q_PROPERTY(QVector3D normal READ normal WRITE setNormal NOTIFY normalChanged)
    QML_NAMED_ELEMENT(HermitePathPoint)
public:
    explicit HermitePathPoint(QObject *parent = nullptr);
    ~HermitePathPoint() override;

    QVector3D position() const;
    void setPosition(const QVector3D &position);

    QVector3D tangent() const;
    void setTangent(const QVector3D &tangent);

    QVector3D normal() const;
    void setNormal(const QVector3D &normal);

signals:
    void positionChanged(const QVector3D &position);
    void tangentChanged(const QVector3D &tangent);
    void normalChanged(const QVector3D &normal);

private:
    QVector3D m_position;
    QVector3D m_tangent;
    QVector3D m_normal = QVector3D(0, 1, 0);
};

class Q_QUICK3DJOLTPHYSICS_EXPORT HermitePath : public PathConstraintPathBase
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<HermitePathPoint> points READ points CONSTANT)
    Q_CLASSINFO("DefaultProperty", "points")
    QML_NAMED_ELEMENT(HermitePath)
public:
    explicit HermitePath(QObject *parent = nullptr);
    ~HermitePath() override;

    QQmlListProperty<HermitePathPoint> points();

    Q_INVOKABLE void addPoint(const QVector3D &position, const QVector3D &tangent,
                              const QVector3D &normal = QVector3D(0, 1, 0));
    Q_INVOKABLE void clearAllPoints();

    int pointCount() const override;

    JPH::Ref<JPH::PathConstraintPath> createJoltPath() const override;

private:
    static void appendPoint(QQmlListProperty<HermitePathPoint> *list, HermitePathPoint *point);
    static qsizetype pointCount(QQmlListProperty<HermitePathPoint> *list);
    static HermitePathPoint *pointAt(QQmlListProperty<HermitePathPoint> *list, qsizetype index);
    static void clearPoints(QQmlListProperty<HermitePathPoint> *list);

    QList<HermitePathPoint *> m_points;
};

#endif // HERMITEPATH_P_H
