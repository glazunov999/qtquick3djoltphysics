#include <QQmlEngine>

#include "../shared/examplecharactercontactlistener.h"
#include "../shared/examplecontactlistener.h"
#include "../shared/examplegeometry.h"
#include "../shared/examplesoftbodycubesettings.h"
#include "../shared/examplesoftbodyspheresettings.h"
#include "../shared/examplesoftbodykinematicspheresettings.h"
#include "../shared/examplesoftbodyclothsettings.h"
#include "../shared/examplesoftbodyforce.h"
#include "../shared/examplesoftbodycontactlistener.h"
#include "../shared/examplesoftbodycubegeometry.h"
#include "../shared/examplesoftbodycubemeshgeometry.h"
#include "../shared/examplesoftbodyspheremeshgeometry.h"
#include "../shared/examplesoftbodyclothmeshgeometry.h"
#include "../shared/examplelayers.h"
#include "../shared/examplesensorcontactlistener.h"

static inline void registerSharedTypes()
{
    qmlRegisterType<ExampleCharacterContactListener>("Example", 1, 0, "ExampleCharacterContactListener");
    qmlRegisterType<ExampleContactListener>("Example", 1, 0, "ExampleContactListener");
    qmlRegisterType<ExampleBoxGeometry>("Example", 1, 0, "ExampleBoxGeometry");
    qmlRegisterType<ExampleTriangleGeometry>("Example", 1, 0, "ExampleTriangleGeometry");
    qmlRegisterType<ExampleMeshGridGeometry>("Example", 1, 0, "ExampleMeshGridGeometry");
    qmlRegisterType<ExampleStairsGridGeometry>("Example", 1, 0, "ExampleStairsGridGeometry");
    qmlRegisterType<ExampleSoftBodyCubeGeometry>("Example", 1, 0, "ExampleSoftBodyCubeGeometry");
    qmlRegisterType<ExampleSoftBodyCubeMeshGeometry>("Example", 1, 0, "ExampleSoftBodyCubeMeshGeometry");
    qmlRegisterType<ExampleSoftBodySphereMeshGeometry>("Example", 1, 0, "ExampleSoftBodySphereMeshGeometry");
    qmlRegisterType<ExampleSoftBodyClothMeshGeometry>("Example", 1, 0, "ExampleSoftBodyClothMeshGeometry");
    qmlRegisterType<ExampleSoftBodyCubeSettings>("Example", 1, 0, "ExampleSoftBodyCubeSettings");
    qmlRegisterType<ExampleSoftBodySphereSettings>("Example", 1, 0, "ExampleSoftBodySphereSettings");
    qmlRegisterType<ExampleSoftBodyKinematicSphereSettings>("Example", 1, 0, "ExampleSoftBodyKinematicSphereSettings");
    qmlRegisterType<ExampleSoftBodyClothSettings>("Example", 1, 0, "ExampleSoftBodyClothSettings");
    qmlRegisterType<ExampleSoftBodyForce>("Example", 1, 0, "ExampleSoftBodyForce");
    qmlRegisterType<ExampleSoftBodyContactListener>("Example", 1, 0, "ExampleSoftBodyContactListener");
    qmlRegisterType<ExampleObjectLayerPairFilter>("Example", 1, 0, "ExampleObjectLayerPairFilter");
    qmlRegisterType<ExampleBroadPhaseLayer>("Example", 1, 0, "ExampleBroadPhaseLayer");
    qmlRegisterType<ExampleObjectVsBroadPhaseLayerFilter>("Example", 1, 0, "ExampleObjectVsBroadPhaseLayerFilter");
    qmlRegisterType<ExampleSensorContactListener>("Example", 1, 0, "ExampleSensorContactListener");
}
