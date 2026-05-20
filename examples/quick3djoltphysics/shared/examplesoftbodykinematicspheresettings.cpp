#include "examplesoftbodykinematicspheresettings.h"

ExampleSoftBodyKinematicSphereSettings::ExampleSoftBodyKinematicSphereSettings(QObject *parent)
    : ExampleSoftBodySphereSettings(parent)
{
    // Base ctor calls rebuildSphere() -> customizeAfterBuild() virtually as the base class,
    // so kinematic vertex settings must be applied again here (see SoftBodyKinematicTest.cpp).
    customizeAfterBuild();
    markTopologyChanged();
}

void ExampleSoftBodyKinematicSphereSettings::customizeAfterBuild()
{
    if (SoftBodyVertex *vertex = topologyVertexAt(0)) {
        vertex->setInverseMass(0.0f);
        vertex->setVelocity(QVector3D(0.0f, 0.0f, 5.0f));
    }
}
