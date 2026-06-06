#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QtQuick3D/qquick3d.h>

#include "mousehandler.h"

#include "../shared/shared.h"
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

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    registerSharedTypes();

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

    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    MouseHandler mouseHandler;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("_mouseHandler", &mouseHandler);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
