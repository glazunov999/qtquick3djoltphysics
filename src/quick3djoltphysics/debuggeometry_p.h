#ifndef DEBUGGEOMETRY_P_H
#define DEBUGGEOMETRY_P_H

#include <QQuick3DGeometry>
#include <QVector3D>

struct DebugLineVertex
{
    QVector3D position;
    quint32 color = 0xffffffff;
};

class DebugGeometry : public QQuick3DGeometry
{
    Q_OBJECT
public:
    explicit DebugGeometry();

    void updateVertices(const QVector<DebugLineVertex> &vertices);

private:
    void clearGeometry();
};

#endif // DEBUGGEOMETRY_P_H
