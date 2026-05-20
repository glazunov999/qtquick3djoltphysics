#include "examplesoftbodyspheresettings.h"

#include <QtMath>

#include <cmath>

namespace {

constexpr float kDefaultEdgeCompliance = 1.0e-4f;
constexpr float kDefaultBendCompliance = 1.0e-3f;

QVector3D unitSpherical(float theta, float phi)
{
    const float sinTheta = std::sin(theta);
    const float cosTheta = std::cos(theta);
    const float sinPhi = std::sin(phi);
    const float cosPhi = std::cos(phi);
    return QVector3D(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
}

int vertexIndex(int numTheta, int numPhi, int theta, int phi)
{
    if (theta == 0)
        return 0;
    if (theta == numTheta - 1)
        return 1;
    return 2 + (theta - 1) * numPhi + (phi % numPhi);
}

} // namespace

ExampleSoftBodySphereSettings::ExampleSoftBodySphereSettings(QObject *parent)
    : SoftBodySharedSettings(parent)
{
    setAutoCreateConstraints(true);
    setEdgeCompliance(kDefaultEdgeCompliance);
    setBendCompliance(kDefaultBendCompliance);
    rebuildSphere();
}

float ExampleSoftBodySphereSettings::radius() const
{
    return m_radius;
}

void ExampleSoftBodySphereSettings::setRadius(float radius)
{
    const float clamped = qMax(0.01f, radius);
    if (qFuzzyCompare(m_radius, clamped))
        return;
    m_radius = clamped;
    emit radiusChanged();
    rebuildSphere();
}

int ExampleSoftBodySphereSettings::numTheta() const
{
    return m_numTheta;
}

void ExampleSoftBodySphereSettings::setNumTheta(int numTheta)
{
    const int clamped = qMax(3, numTheta);
    if (m_numTheta == clamped)
        return;
    m_numTheta = clamped;
    emit numThetaChanged();
    rebuildSphere();
}

int ExampleSoftBodySphereSettings::numPhi() const
{
    return m_numPhi;
}

void ExampleSoftBodySphereSettings::setNumPhi(int numPhi)
{
    const int clamped = qMax(3, numPhi);
    if (m_numPhi == clamped)
        return;
    m_numPhi = clamped;
    emit numPhiChanged();
    rebuildSphere();
}

void ExampleSoftBodySphereSettings::buildSphere(float radius, int numTheta, int numPhi)
{
    if (radius > 0.0f)
        m_radius = radius;
    if (numTheta >= 3)
        m_numTheta = numTheta;
    if (numPhi >= 3)
        m_numPhi = numPhi;
    rebuildSphere();
}

void ExampleSoftBodySphereSettings::rebuildSphere()
{
    const float radius = qMax(0.01f, m_radius);
    const int numTheta = qMax(3, m_numTheta);
    const int numPhi = qMax(3, m_numPhi);

    clearTopology();

    addVertex(radius * unitSpherical(0.0f, 0.0f), 1.0f, QVector2D(0.5f, 0.0f));
    addVertex(radius * unitSpherical(float(M_PI), 0.0f), 1.0f, QVector2D(0.5f, 1.0f));

    for (int theta = 1; theta < numTheta - 1; ++theta) {
        for (int phi = 0; phi < numPhi; ++phi) {
            const float t = float(M_PI) * float(theta) / float(numTheta - 1);
            const float p = 2.0f * float(M_PI) * float(phi) / float(numPhi);
            const float u = float(phi) / float(numPhi);
            const float v = float(theta) / float(numTheta - 1);
            addVertex(radius * unitSpherical(t, p), 1.0f, QVector2D(u, v));
        }
    }

    for (int phi = 0; phi < numPhi; ++phi) {
        for (int theta = 0; theta < numTheta - 2; ++theta) {
            addFace(vertexIndex(numTheta, numPhi, theta, phi),
                    vertexIndex(numTheta, numPhi, theta + 1, phi),
                    vertexIndex(numTheta, numPhi, theta + 1, phi + 1));

            if (theta > 0) {
                addFace(vertexIndex(numTheta, numPhi, theta + 1, phi + 1),
                        vertexIndex(numTheta, numPhi, theta, phi + 1),
                        vertexIndex(numTheta, numPhi, theta, phi));
            }
        }

        addFace(vertexIndex(numTheta, numPhi, numTheta - 2, phi + 1),
                vertexIndex(numTheta, numPhi, numTheta - 2, phi),
                vertexIndex(numTheta, numPhi, numTheta - 1, 0));
    }

    customizeAfterBuild();
    markTopologyChanged();
}

void ExampleSoftBodySphereSettings::customizeAfterBuild()
{
}
