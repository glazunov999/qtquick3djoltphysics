#ifndef EXAMPLESOFTBODYCUBEGEOMETRY_H
#define EXAMPLESOFTBODYCUBEGEOMETRY_H

#include <QQuick3DGeometry>

class ExampleSoftBodyCubeGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(float gridSpacing READ gridSpacing WRITE setGridSpacing NOTIFY gridSpacingChanged)

public:
    explicit ExampleSoftBodyCubeGeometry();

    int gridSize() const;
    void setGridSize(int gridSize);

    float gridSpacing() const;
    void setGridSpacing(float gridSpacing);

signals:
    void gridSizeChanged();
    void gridSpacingChanged();

private:
    void updateData();

    int m_gridSize = 5;
    float m_gridSpacing = 0.5f;
};

#endif // EXAMPLESOFTBODYCUBEGEOMETRY_H
