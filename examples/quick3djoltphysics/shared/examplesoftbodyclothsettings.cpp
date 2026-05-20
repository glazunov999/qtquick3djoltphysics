#include "examplesoftbodyclothsettings.h"

#include <QtMath>

namespace {

constexpr float kClothEdgeCompliance = 1.0e-5f;
constexpr float kClothBendCompliance = 1.0e-5f;

int vertexIndex(int gridSizeX, int x, int z)
{
    return x + z * gridSizeX;
}

} // namespace

ExampleSoftBodyClothSettings::ExampleSoftBodyClothSettings(QObject *parent)
    : SoftBodySharedSettings(parent)
{
    setAutoCreateConstraints(true);
    setEdgeCompliance(kClothEdgeCompliance);
    setBendCompliance(kClothBendCompliance);
    rebuildCloth();
}

int ExampleSoftBodyClothSettings::gridSizeX() const
{
    return m_gridSizeX;
}

void ExampleSoftBodyClothSettings::setGridSizeX(int gridSizeX)
{
    const int clamped = qMax(2, gridSizeX);
    if (m_gridSizeX == clamped)
        return;
    m_gridSizeX = clamped;
    emit gridSizeXChanged();
    rebuildCloth();
}

int ExampleSoftBodyClothSettings::gridSizeZ() const
{
    return m_gridSizeZ;
}

void ExampleSoftBodyClothSettings::setGridSizeZ(int gridSizeZ)
{
    const int clamped = qMax(2, gridSizeZ);
    if (m_gridSizeZ == clamped)
        return;
    m_gridSizeZ = clamped;
    emit gridSizeZChanged();
    rebuildCloth();
}

float ExampleSoftBodyClothSettings::gridSpacing() const
{
    return m_gridSpacing;
}

void ExampleSoftBodyClothSettings::setGridSpacing(float gridSpacing)
{
    const float clamped = qMax(0.01f, gridSpacing);
    if (qFuzzyCompare(m_gridSpacing, clamped))
        return;
    m_gridSpacing = clamped;
    emit gridSpacingChanged();
    rebuildCloth();
}

bool ExampleSoftBodyClothSettings::pinTwoFrontCorners() const
{
    return m_pinTwoFrontCorners;
}

void ExampleSoftBodyClothSettings::setPinTwoFrontCorners(bool pin)
{
    if (m_pinTwoFrontCorners == pin)
        return;
    m_pinTwoFrontCorners = pin;
    emit pinTwoFrontCornersChanged();
    rebuildCloth();
}

bool ExampleSoftBodyClothSettings::pinFourCorners() const
{
    return m_pinFourCorners;
}

void ExampleSoftBodyClothSettings::setPinFourCorners(bool pin)
{
    if (m_pinFourCorners == pin)
        return;
    m_pinFourCorners = pin;
    emit pinFourCornersChanged();
    rebuildCloth();
}

void ExampleSoftBodyClothSettings::buildCloth(int gridSizeX, int gridSizeZ, float gridSpacing)
{
    if (gridSizeX >= 2)
        m_gridSizeX = gridSizeX;
    if (gridSizeZ >= 2)
        m_gridSizeZ = gridSizeZ;
    if (gridSpacing > 0.0f)
        m_gridSpacing = gridSpacing;
    rebuildCloth();
}

void ExampleSoftBodyClothSettings::rebuildCloth()
{
    const int gridX = qMax(2, m_gridSizeX);
    const int gridZ = qMax(2, m_gridSizeZ);
    const float spacing = qMax(0.01f, m_gridSpacing);
    const float offsetX = -0.5f * spacing * (gridX - 1);
    const float offsetZ = -0.5f * spacing * (gridZ - 1);

    clearTopology();

    for (int z = 0; z < gridZ; ++z) {
        for (int x = 0; x < gridX; ++x) {
            const float u = gridX > 1 ? float(x) / float(gridX - 1) : 0.5f;
            const float v = gridZ > 1 ? 1.0f - float(z) / float(gridZ - 1) : 0.5f;
            addVertex(QVector3D(offsetX + spacing * x, 0.0f, offsetZ + spacing * z),
                      1.0f,
                      QVector2D(u, v));
        }
    }

    for (int z = 0; z < gridZ - 1; ++z) {
        for (int x = 0; x < gridX - 1; ++x) {
            addFace(vertexIndex(gridX, x, z),
                    vertexIndex(gridX, x, z + 1),
                    vertexIndex(gridX, x + 1, z + 1));
            addFace(vertexIndex(gridX, x + 1, z + 1),
                    vertexIndex(gridX, x + 1, z),
                    vertexIndex(gridX, x, z));
        }
    }

    customizeAfterBuild();
    markTopologyChanged();
}

void ExampleSoftBodyClothSettings::customizeAfterBuild()
{
    const int gridX = qMax(2, m_gridSizeX);
    const int gridZ = qMax(2, m_gridSizeZ);

    if (m_pinFourCorners) {
        if (SoftBodyVertex *v = topologyVertexAt(vertexIndex(gridX, 0, 0)))
            v->setInverseMass(0.0f);
        if (SoftBodyVertex *v = topologyVertexAt(vertexIndex(gridX, gridX - 1, 0)))
            v->setInverseMass(0.0f);
        if (SoftBodyVertex *v = topologyVertexAt(vertexIndex(gridX, 0, gridZ - 1)))
            v->setInverseMass(0.0f);
        if (SoftBodyVertex *v = topologyVertexAt(vertexIndex(gridX, gridX - 1, gridZ - 1)))
            v->setInverseMass(0.0f);
    }

    if (!m_pinTwoFrontCorners)
        return;

    if (SoftBodyVertex *v00 = topologyVertexAt(vertexIndex(gridX, 0, 0)))
        v00->setInverseMass(0.0f);
    if (SoftBodyVertex *v10 = topologyVertexAt(vertexIndex(gridX, gridX - 1, 0)))
        v10->setInverseMass(0.0f);
}
