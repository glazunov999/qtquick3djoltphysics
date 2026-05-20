#include "hermitepath_p.h"
#include "physicsutils_p.h"

#include <utility>

HermitePathPoint::HermitePathPoint(QObject *parent) : QObject(parent)
{
}

HermitePathPoint::~HermitePathPoint() = default;

QVector3D HermitePathPoint::position() const
{
    return m_position;
}

void HermitePathPoint::setPosition(const QVector3D &position)
{
    if (m_position == position)
        return;

    m_position = position;
    emit positionChanged(m_position);
}

QVector3D HermitePathPoint::tangent() const
{
    return m_tangent;
}

void HermitePathPoint::setTangent(const QVector3D &tangent)
{
    if (m_tangent == tangent)
        return;

    m_tangent = tangent;
    emit tangentChanged(m_tangent);
}

QVector3D HermitePathPoint::normal() const
{
    return m_normal;
}

void HermitePathPoint::setNormal(const QVector3D &normal)
{
    if (m_normal == normal)
        return;

    m_normal = normal;
    emit normalChanged(m_normal);
}

HermitePath::HermitePath(QObject *parent) : PathConstraintPathBase(parent)
{
}

HermitePath::~HermitePath() = default;

QQmlListProperty<HermitePathPoint> HermitePath::points()
{
    return QQmlListProperty<HermitePathPoint>(this, nullptr, &HermitePath::appendPoint,
                                              &HermitePath::pointCount, &HermitePath::pointAt,
                                              &HermitePath::clearPoints);
}

void HermitePath::addPoint(const QVector3D &position, const QVector3D &tangent,
                           const QVector3D &normal)
{
    auto *point = new HermitePathPoint(this);
    point->setPosition(position);
    point->setTangent(tangent);
    point->setNormal(normal);
    m_points.append(point);
    emit changed();
}

void HermitePath::clearAllPoints()
{
    if (m_points.isEmpty())
        return;

    qDeleteAll(m_points);
    m_points.clear();
    emit changed();
}

int HermitePath::pointCount() const
{
    return m_points.size();
}

JPH::Ref<JPH::PathConstraintPath> HermitePath::createJoltPath() const
{
    auto *hermitePath = new JPH::PathConstraintPathHermite();
    hermitePath->SetIsLooping(m_isLooping);

    for (const auto *point : std::as_const(m_points)) {
        hermitePath->AddPoint(PhysicsUtils::toJoltType(point->position()),
                              PhysicsUtils::toJoltType(point->tangent()),
                              PhysicsUtils::toJoltType(point->normal()));
    }

    return hermitePath;
}

void HermitePath::appendPoint(QQmlListProperty<HermitePathPoint> *list, HermitePathPoint *point)
{
    auto *self = static_cast<HermitePath *>(list->object);
    self->m_points.append(point);
    emit self->changed();
}

qsizetype HermitePath::pointCount(QQmlListProperty<HermitePathPoint> *list)
{
    auto *self = static_cast<HermitePath *>(list->object);
    return self->m_points.size();
}

HermitePathPoint *HermitePath::pointAt(QQmlListProperty<HermitePathPoint> *list, qsizetype index)
{
    auto *self = static_cast<HermitePath *>(list->object);
    return self->m_points.at(index);
}

void HermitePath::clearPoints(QQmlListProperty<HermitePathPoint> *list)
{
    auto *self = static_cast<HermitePath *>(list->object);
    self->clearAllPoints();
}
