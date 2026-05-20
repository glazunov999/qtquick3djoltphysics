#ifndef EXAMPLESOFTBODYSPHEREMESHGEOMETRY_H
#define EXAMPLESOFTBODYSPHEREMESHGEOMETRY_H

#include <QtQuick3DJoltPhysics/softbodymeshgeometry.h>
#include <QVector>
#include <QVector2D>

class ExampleSoftBodySphereMeshGeometry : public SoftBodyMeshGeometry
{
    Q_OBJECT
public:
    explicit ExampleSoftBodySphereMeshGeometry();

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
    bool m_restUvsValid = false;
    QVector<QVector2D> m_restUvs;
};

#endif // EXAMPLESOFTBODYSPHEREMESHGEOMETRY_H
