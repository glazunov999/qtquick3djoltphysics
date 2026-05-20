#include "examplesoftbodycubesettings.h"

#include <QtMath>

namespace {

int vertexIndex(int grid, int x, int y, int z)
{
    return x + y * grid + z * grid * grid;
}

enum class CubeFace {
    NegX,
    PosX,
    NegY,
    PosY,
    NegZ,
    PosZ
};

CubeFace cubeFaceFromNormal(const QVector3D &n)
{
    const float absX = qAbs(n.x());
    const float absY = qAbs(n.y());
    const float absZ = qAbs(n.z());

    if (absY >= absX && absY >= absZ)
        return n.y() >= 0.0f ? CubeFace::PosY : CubeFace::NegY;
    if (absX >= absZ)
        return n.x() >= 0.0f ? CubeFace::PosX : CubeFace::NegX;
    return n.z() >= 0.0f ? CubeFace::PosZ : CubeFace::NegZ;
}

void cubeBounds(int grid, float spacing, QVector3D &boundsMin, QVector3D &boundsMax)
{
    const float offset = -0.5f * spacing * float(qMax(2, grid) - 1);
    boundsMin = QVector3D(offset, offset, offset);
    boundsMax = QVector3D(-offset, -offset, -offset);
}

QVector2D cubeFaceTextureCoordinate(CubeFace face,
                                    const QVector3D &p,
                                    const QVector3D &boundsMin,
                                    const QVector3D &boundsMax)
{
    const float spanX = boundsMax.x() - boundsMin.x();
    const float spanY = boundsMax.y() - boundsMin.y();
    const float spanZ = boundsMax.z() - boundsMin.z();

    switch (face) {
    case CubeFace::NegX: {
        const float u = spanZ > 0.0f ? (p.z() - boundsMin.z()) / spanZ : 0.5f;
        const float v = spanY > 0.0f ? (p.y() - boundsMin.y()) / spanY : 0.5f;
        return QVector2D(1.0f - u, v);
    }
    case CubeFace::PosX: {
        const float u = spanZ > 0.0f ? (p.z() - boundsMin.z()) / spanZ : 0.5f;
        const float v = spanY > 0.0f ? (p.y() - boundsMin.y()) / spanY : 0.5f;
        return QVector2D(u, v);
    }
    case CubeFace::NegY: {
        const float u = spanX > 0.0f ? (p.x() - boundsMin.x()) / spanX : 0.5f;
        const float v = spanZ > 0.0f ? (p.z() - boundsMin.z()) / spanZ : 0.5f;
        return QVector2D(u, 1.0f - v);
    }
    case CubeFace::PosY: {
        const float u = spanX > 0.0f ? (p.x() - boundsMin.x()) / spanX : 0.5f;
        const float v = spanZ > 0.0f ? (p.z() - boundsMin.z()) / spanZ : 0.5f;
        return QVector2D(u, v);
    }
    case CubeFace::NegZ: {
        const float u = spanX > 0.0f ? (p.x() - boundsMin.x()) / spanX : 0.5f;
        const float v = spanY > 0.0f ? (p.y() - boundsMin.y()) / spanY : 0.5f;
        return QVector2D(1.0f - u, v);
    }
    case CubeFace::PosZ: {
        const float u = spanX > 0.0f ? (p.x() - boundsMin.x()) / spanX : 0.5f;
        const float v = spanY > 0.0f ? (p.y() - boundsMin.y()) / spanY : 0.5f;
        return QVector2D(u, v);
    }
    }

    return {};
}

} // namespace

ExampleSoftBodyCubeSettings::ExampleSoftBodyCubeSettings(QObject *parent)
    : SoftBodySharedSettings(parent)
{
    setAutoCreateConstraints(false);
    rebuildCube();
}

bool ExampleSoftBodyCubeSettings::usesRenderingTextureCoordinates() const
{
    return true;
}

void ExampleSoftBodyCubeSettings::cacheRestPositions(const QVector<QVector3D> &restPositions)
{
    cacheRestPositionsForRendering(restPositions);
}

QVector2D ExampleSoftBodyCubeSettings::textureCoordinateForFace(int vertexIndex,
                                                                  const QVector3D &restPosition,
                                                                  const QVector3D &restFaceNormal) const
{
    return textureCoordinateForRendering(vertexIndex, restPosition, restFaceNormal);
}

QVector2D ExampleSoftBodyCubeSettings::textureCoordinateForRendering(int vertexIndex,
                                                                     const QVector3D &restPosition,
                                                                     const QVector3D &restFaceNormal) const
{
    Q_UNUSED(vertexIndex)

    QVector3D boundsMin;
    QVector3D boundsMax;
    cubeBounds(m_gridSize, m_gridSpacing, boundsMin, boundsMax);

    return cubeFaceTextureCoordinate(cubeFaceFromNormal(restFaceNormal),
                                     restPosition,
                                     boundsMin,
                                     boundsMax);
}

int ExampleSoftBodyCubeSettings::gridSize() const
{
    return m_gridSize;
}

void ExampleSoftBodyCubeSettings::setGridSize(int gridSize)
{
    const int clamped = qMax(2, gridSize);
    if (m_gridSize == clamped)
        return;
    m_gridSize = clamped;
    emit gridSizeChanged();
    rebuildCube();
}

float ExampleSoftBodyCubeSettings::gridSpacing() const
{
    return m_gridSpacing;
}

void ExampleSoftBodyCubeSettings::setGridSpacing(float gridSpacing)
{
    const float clamped = qMax(0.01f, gridSpacing);
    if (qFuzzyCompare(m_gridSpacing, clamped))
        return;
    m_gridSpacing = clamped;
    emit gridSpacingChanged();
    rebuildCube();
}

void ExampleSoftBodyCubeSettings::buildCube(int gridSize, float gridSpacing)
{
    if (gridSize >= 2)
        m_gridSize = gridSize;
    if (gridSpacing > 0.0f)
        m_gridSpacing = gridSpacing;
    rebuildCube();
}

void ExampleSoftBodyCubeSettings::rebuildCube()
{
    const int grid = qMax(2, m_gridSize);
    const float spacing = qMax(0.01f, m_gridSpacing);
    const float offset = -0.5f * spacing * (grid - 1);

    clearTopology();

    for (int z = 0; z < grid; ++z) {
        for (int y = 0; y < grid; ++y) {
            for (int x = 0; x < grid; ++x) {
                addVertex(QVector3D(offset + spacing * float(x),
                                    offset + spacing * float(y),
                                    offset + spacing * float(z)));
            }
        }
    }

    for (int z = 0; z < grid; ++z) {
        for (int y = 0; y < grid; ++y) {
            for (int x = 0; x < grid; ++x) {
                const int v0 = vertexIndex(grid, x, y, z);
                if (x < grid - 1)
                    addEdge(v0, vertexIndex(grid, x + 1, y, z));
                if (y < grid - 1)
                    addEdge(v0, vertexIndex(grid, x, y + 1, z));
                if (z < grid - 1)
                    addEdge(v0, vertexIndex(grid, x, y, z + 1));
            }
        }
    }

    static const int tetraIndices[6][4][3] = {
        { { 0, 0, 0 }, { 0, 1, 1 }, { 0, 0, 1 }, { 1, 1, 1 } },
        { { 0, 0, 0 }, { 0, 1, 0 }, { 0, 1, 1 }, { 1, 1, 1 } },
        { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 } },
        { { 0, 0, 0 }, { 1, 0, 1 }, { 1, 0, 0 }, { 1, 1, 1 } },
        { { 0, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 }, { 1, 1, 1 } },
        { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 1, 1, 1 } },
    };

    for (int z = 0; z < grid - 1; ++z) {
        for (int y = 0; y < grid - 1; ++y) {
            for (int x = 0; x < grid - 1; ++x) {
                for (int t = 0; t < 6; ++t) {
                    addVolume(vertexIndex(grid, x + tetraIndices[t][0][0],
                                          y + tetraIndices[t][0][1],
                                          z + tetraIndices[t][0][2]),
                              vertexIndex(grid, x + tetraIndices[t][1][0],
                                          y + tetraIndices[t][1][1],
                                          z + tetraIndices[t][1][2]),
                              vertexIndex(grid, x + tetraIndices[t][2][0],
                                          y + tetraIndices[t][2][1],
                                          z + tetraIndices[t][2][2]),
                              vertexIndex(grid, x + tetraIndices[t][3][0],
                                          y + tetraIndices[t][3][1],
                                          z + tetraIndices[t][3][2]));
                }
            }
        }
    }

    for (int y = 0; y < grid - 1; ++y) {
        for (int x = 0; x < grid - 1; ++x) {
            addFace(vertexIndex(grid, x, y, 0),
                    vertexIndex(grid, x, y + 1, 0),
                    vertexIndex(grid, x + 1, y + 1, 0));
            addFace(vertexIndex(grid, x + 1, y + 1, 0),
                    vertexIndex(grid, x + 1, y, 0),
                    vertexIndex(grid, x, y, 0));

            addFace(vertexIndex(grid, x, y, grid - 1),
                    vertexIndex(grid, x + 1, y + 1, grid - 1),
                    vertexIndex(grid, x, y + 1, grid - 1));
            addFace(vertexIndex(grid, x + 1, y, grid - 1),
                    vertexIndex(grid, x + 1, y + 1, grid - 1),
                    vertexIndex(grid, x, y, grid - 1));

            addFace(vertexIndex(grid, x, 0, y),
                    vertexIndex(grid, x + 1, 0, y + 1),
                    vertexIndex(grid, x, 0, y + 1));
            addFace(vertexIndex(grid, x + 1, 0, y),
                    vertexIndex(grid, x + 1, 0, y + 1),
                    vertexIndex(grid, x, 0, y));

            addFace(vertexIndex(grid, x, grid - 1, y),
                    vertexIndex(grid, x, grid - 1, y + 1),
                    vertexIndex(grid, x + 1, grid - 1, y + 1));
            addFace(vertexIndex(grid, x + 1, grid - 1, y + 1),
                    vertexIndex(grid, x + 1, grid - 1, y),
                    vertexIndex(grid, x, grid - 1, y));

            addFace(vertexIndex(grid, 0, x, y),
                    vertexIndex(grid, 0, x, y + 1),
                    vertexIndex(grid, 0, x + 1, y + 1));
            addFace(vertexIndex(grid, 0, x + 1, y + 1),
                    vertexIndex(grid, 0, x + 1, y),
                    vertexIndex(grid, 0, x, y));

            addFace(vertexIndex(grid, grid - 1, x, y),
                    vertexIndex(grid, grid - 1, x + 1, y + 1),
                    vertexIndex(grid, grid - 1, x, y + 1));
            addFace(vertexIndex(grid, grid - 1, x + 1, y),
                    vertexIndex(grid, grid - 1, x + 1, y + 1),
                    vertexIndex(grid, grid - 1, x, y));
        }
    }

    markTopologyChanged();
}
