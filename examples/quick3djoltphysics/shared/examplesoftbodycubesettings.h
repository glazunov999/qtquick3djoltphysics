#ifndef EXAMPLESOFTBODYCUBESETTINGS_H
#define EXAMPLESOFTBODYCUBESETTINGS_H

#include <QtQuick3DJoltPhysics/softbodysharedsettings.h>

class ExampleSoftBodyCubeSettings : public SoftBodySharedSettings
{
    Q_OBJECT
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(float gridSpacing READ gridSpacing WRITE setGridSpacing NOTIFY gridSpacingChanged)
public:
    explicit ExampleSoftBodyCubeSettings(QObject *parent = nullptr);

    int gridSize() const;
    void setGridSize(int gridSize);

    float gridSpacing() const;
    void setGridSpacing(float gridSpacing);

    Q_INVOKABLE void buildCube(int gridSize = -1, float gridSpacing = -1.0f);

    void cacheRestPositions(const QVector<QVector3D> &restPositions);
    QVector2D textureCoordinateForFace(int vertexIndex,
                                       const QVector3D &restPosition,
                                       const QVector3D &restFaceNormal) const;

signals:
    void gridSizeChanged();
    void gridSpacingChanged();

protected:
    bool usesRenderingTextureCoordinates() const override;
    QVector2D textureCoordinateForRendering(int vertexIndex,
                                            const QVector3D &restPosition,
                                            const QVector3D &restFaceNormal) const override;

private:
    void rebuildCube();

    int m_gridSize = 5;
    float m_gridSpacing = 0.5f;
};

#endif // EXAMPLESOFTBODYCUBESETTINGS_H
