#include "examplesoftbodycubegeometry.h"

#include <QVector3D>
#include <QtMath>

namespace {

QVector3D latticePosition(int x, int y, int z, int gridSize, float gridSpacing)
{
    const float offset = -0.5f * gridSpacing * (float(gridSize) - 1.0f);
    return QVector3D(offset + float(x) * gridSpacing,
                     offset + float(y) * gridSpacing,
                     offset + float(z) * gridSpacing);
}

int vertexIndex(int x, int y, int z, int gridSize)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

void addTriangle(QVector<QVector3D> &vertices,
                 QVector3D a,
                 QVector3D b,
                 QVector3D c)
{
    vertices.append(a);
    vertices.append(b);
    vertices.append(c);
}

} // namespace

ExampleSoftBodyCubeGeometry::ExampleSoftBodyCubeGeometry()
{
    updateData();
}

int ExampleSoftBodyCubeGeometry::gridSize() const
{
    return m_gridSize;
}

void ExampleSoftBodyCubeGeometry::setGridSize(int gridSize)
{
    const int clamped = qMax(2, gridSize);
    if (m_gridSize == clamped)
        return;
    m_gridSize = clamped;
    updateData();
    emit gridSizeChanged();
}

float ExampleSoftBodyCubeGeometry::gridSpacing() const
{
    return m_gridSpacing;
}

void ExampleSoftBodyCubeGeometry::setGridSpacing(float gridSpacing)
{
    const float clamped = qMax(0.01f, gridSpacing);
    if (qFuzzyCompare(m_gridSpacing, clamped))
        return;
    m_gridSpacing = clamped;
    updateData();
    emit gridSpacingChanged();
}

void ExampleSoftBodyCubeGeometry::updateData()
{
    clear();

    const int gridSize = m_gridSize;
    const float gridSpacing = m_gridSpacing;

    QVector<QVector3D> lattice;
    lattice.reserve(gridSize * gridSize * gridSize);
    for (int z = 0; z < gridSize; ++z) {
        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x)
                lattice.append(latticePosition(x, y, z, gridSize, gridSpacing));
        }
    }

    QVector<QVector3D> triangles;
    triangles.reserve(12 * (gridSize - 1) * (gridSize - 1) * 3);

    const auto vtx = [&](int x, int y, int z) {
        return lattice.at(vertexIndex(x, y, z, gridSize));
    };

    for (int y = 0; y < gridSize - 1; ++y) {
        for (int x = 0; x < gridSize - 1; ++x) {
            addTriangle(triangles, vtx(x, y, 0), vtx(x, y + 1, 0), vtx(x + 1, y + 1, 0));
            addTriangle(triangles, vtx(x, y, 0), vtx(x + 1, y + 1, 0), vtx(x + 1, y, 0));

            addTriangle(triangles,
                        vtx(x, y, gridSize - 1),
                        vtx(x + 1, y + 1, gridSize - 1),
                        vtx(x, y + 1, gridSize - 1));
            addTriangle(triangles,
                        vtx(x, y, gridSize - 1),
                        vtx(x + 1, y, gridSize - 1),
                        vtx(x + 1, y + 1, gridSize - 1));

            addTriangle(triangles, vtx(x, 0, y), vtx(x + 1, 0, y + 1), vtx(x, 0, y + 1));
            addTriangle(triangles, vtx(x, 0, y), vtx(x + 1, 0, y), vtx(x + 1, 0, y + 1));

            addTriangle(triangles,
                        vtx(x, gridSize - 1, y),
                        vtx(x, gridSize - 1, y + 1),
                        vtx(x + 1, gridSize - 1, y + 1));
            addTriangle(triangles,
                        vtx(x, gridSize - 1, y),
                        vtx(x + 1, gridSize - 1, y + 1),
                        vtx(x + 1, gridSize - 1, y));

            addTriangle(triangles, vtx(0, x, y), vtx(0, x, y + 1), vtx(0, x + 1, y + 1));
            addTriangle(triangles, vtx(0, x, y), vtx(0, x + 1, y + 1), vtx(0, x + 1, y));

            addTriangle(triangles,
                        vtx(gridSize - 1, x, y),
                        vtx(gridSize - 1, x + 1, y + 1),
                        vtx(gridSize - 1, x, y + 1));
            addTriangle(triangles,
                        vtx(gridSize - 1, x, y),
                        vtx(gridSize - 1, x + 1, y),
                        vtx(gridSize - 1, x + 1, y + 1));
        }
    }

    constexpr int kStride = sizeof(QVector3D);
    QByteArray vertexData(triangles.size() * kStride, Qt::Initialization::Uninitialized);
    QVector3D *p = reinterpret_cast<QVector3D *>(vertexData.data());
    for (const QVector3D &v : std::as_const(triangles))
        *p++ = v;

    QVector3D boundsMin = lattice.first();
    QVector3D boundsMax = lattice.first();
    for (const QVector3D &v : lattice) {
        boundsMin.setX(qMin(boundsMin.x(), v.x()));
        boundsMin.setY(qMin(boundsMin.y(), v.y()));
        boundsMin.setZ(qMin(boundsMin.z(), v.z()));
        boundsMax.setX(qMax(boundsMax.x(), v.x()));
        boundsMax.setY(qMax(boundsMax.y(), v.y()));
        boundsMax.setZ(qMax(boundsMax.z(), v.z()));
    }

    setVertexData(vertexData);
    setStride(kStride);
    setBounds(boundsMin, boundsMax);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}
