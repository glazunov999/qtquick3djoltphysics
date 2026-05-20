#ifndef SOFTBODYMESHGEOMETRY_H
#define SOFTBODYMESHGEOMETRY_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>
#include <QQuick3DGeometry>
#include <QVector2D>
#include <QVector3D>

class SoftBody;
class SoftBodySharedSettings;

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodyMeshGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    Q_PROPERTY(SoftBody *softBody READ softBody WRITE setSoftBody NOTIFY softBodyChanged)
    QML_NAMED_ELEMENT(SoftBodyMeshGeometry)
public:
    explicit SoftBodyMeshGeometry();

    SoftBody *softBody() const;
    void setSoftBody(SoftBody *softBody);

    virtual void invalidateRestUvs();
    virtual void updateFromSoftBody();

signals:
    void softBodyChanged();

protected:
    virtual void ensureCachedData(const QVector<QVector3D> &restPositions,
                                  SoftBodySharedSettings *settings);
    virtual void textureCoordinatesForTriangle(int i0,
                                               int i1,
                                               int i2,
                                               SoftBodySharedSettings *settings,
                                               QVector2D &uv0,
                                               QVector2D &uv1,
                                               QVector2D &uv2) const;

    void updateTopologyConnection();

    SoftBody *m_softBody = nullptr;
    SoftBodySharedSettings *m_topologySettings = nullptr;

private:
    void removeTopologyConnection();
};

#endif // SOFTBODYMESHGEOMETRY_H
