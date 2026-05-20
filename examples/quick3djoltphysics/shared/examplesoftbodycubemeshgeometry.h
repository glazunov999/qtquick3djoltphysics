#ifndef EXAMPLESOFTBODYCUBEMESHGEOMETRY_H
#define EXAMPLESOFTBODYCUBEMESHGEOMETRY_H

#include <QtQuick3DJoltPhysics/softbodymeshgeometry.h>
#include <QVector>
#include <QVector3D>

class ExampleSoftBodyCubeMeshGeometry : public SoftBodyMeshGeometry
{
    Q_OBJECT
public:
    explicit ExampleSoftBodyCubeMeshGeometry();

protected:
    void invalidateRestUvs() override;
    void ensureCachedData(const QVector<QVector3D> &restPositions,
                          SoftBodySharedSettings *settings) override;
    void textureCoordinatesForTriangle(int i0,
                                       int i1,
                                       int i2,
                                       SoftBodySharedSettings *settings,
                                       QVector2D &uv0,
                                       QVector2D &uv1,
                                       QVector2D &uv2) const override;

private:
    bool m_restPositionsValid = false;
    QVector<QVector3D> m_restPositions;
};

#endif // EXAMPLESOFTBODYCUBEMESHGEOMETRY_H
