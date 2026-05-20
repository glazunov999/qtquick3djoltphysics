#include "examplesoftbodyforce.h"

#include "exampleperlin.h"

#include <cmath>

ExampleSoftBodyForce::ExampleSoftBodyForce(QObject *parent)
    : QObject(parent)
{
}

QVector3D ExampleSoftBodyForce::fluctuatingForce(float time, float maxForce, float maxAngleDegrees) const
{
    constexpr int wrap = 256;

    QVector3D force(0.0f, 0.0f,
                    0.5f * maxForce * (1.0f + examplePerlinNoise3(0.0f, 0.0f, time / 2.0f, wrap, wrap, wrap)));

    const float angle = qDegreesToRadians(maxAngleDegrees)
            * examplePerlinNoise3(time / 10.0f, 0.0f, 0.0f, wrap, wrap, wrap);

    const float cosA = std::cos(angle);
    const float sinA = std::sin(angle);
    return QVector3D(force.z() * sinA, 0.0f, force.z() * cosA);
}
