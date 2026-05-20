import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.JoltPhysics
import Example

import ".."

Item {
    id: mainWindow

    anchors.fill: parent

    readonly property int nonMoving: 0
    readonly property int moving: 1

    // Jolt Samples/Tests/SoftBody/SoftBodyRestitutionTest
    readonly property int bodyCount: 11
    readonly property real rowStartX: -50
    readonly property real rowStep: 10
    readonly property real rowY: 10
    readonly property real sphereRowZ: 0
    readonly property real cubeRowZ: -5

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: AppSettings.playPhysics
        time: timer.elapsedTime * 1000
        gravity: Qt.vector3d(0, -AppSettings.gravity, 0)
        numThreads: AppSettings.maxConcurrentJobs
        collisionSteps: AppSettings.collisionSteps
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        settings: PhysicsSettings {
            numVelocitySteps: Math.round(AppSettings.numVelocitySteps)
            numPositionSteps: Math.round(AppSettings.numPositionSteps)
            baumgarte: AppSettings.baumgarte
            speculativeContactDistance: AppSettings.speculativeContactDistance
            penetrationSlop: AppSettings.penetrationSlop
            linearCastThreshold: AppSettings.linearCastThreshold
            minVelocityForRestitution: AppSettings.minVelocityForRestitution
            timeBeforeSleep: AppSettings.timeBeforeSleep
            pointVelocitySleepThreshold: AppSettings.pointVelocitySleepThreshold
            deterministicSimulation: AppSettings.deterministicSimulation
            constraintWarmStart: AppSettings.constraintWarmStart
            useBodyPairContactCache: AppSettings.useBodyPairContactCache
            useManifoldReduction: AppSettings.useManifoldReduction
            useLargeIslandSplitter: AppSettings.useLargeIslandSplitter
            allowSleeping: AppSettings.allowSleeping
            checkActiveEdges: AppSettings.checkActiveEdges
        }
    }

    FrameAnimation {
        id: timer
        running: AppSettings.playPhysics
    }

    SampleCameraController {
        camera: camera
        physicsSystem: physicsSystem
        speed: 0.1
        shiftSpeed: 0.3
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#191970"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(30, 10, 30)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(-30, -10, -30))
            }
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            brightness: 1.5
            castsShadow: true
            shadowFactor: 75
            shadowMapQuality: Light.ShadowMapQualityVeryHigh
            pcfFactor: 0.01
            shadowBias: 0.1
        }

        Body {
            shape: BoxShape {
                extents: Qt.vector3d(200, 2, 200)
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, -1, 0)
            restitution: 0
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: "white"
                    metalness: 0.5
                    roughness: 0.1
                }
                castsShadows: false
                receivesShadows: true
                scale: Qt.vector3d(2, 0.02, 2)
            }
        }

        // Spheres: CreateSphere(), pressure 2000, restitution 0.1 * i, z = 0
        Repeater3D {
            model: mainWindow.bodyCount
            SoftBody {
                id: sphereBody
                position: Qt.vector3d(mainWindow.rowStartX + index * mainWindow.rowStep,
                                      mainWindow.rowY,
                                      mainWindow.sphereRowZ)
                objectLayer: moving
                restitution: 0.1 * index
                pressure: 2000
                allowSleeping: false
                sharedSettings: ExampleSoftBodySphereSettings {}

                Model {
                    geometry: ExampleSoftBodySphereMeshGeometry {
                        softBody: sphereBody
                    }
                    materials: PrincipledMaterial {
                        cullMode: PrincipledMaterial.NoCulling
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[(sphereBody.bodyID + 1) % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        // Cubes: CreateCube(5, 0.5), restitution 0.1 * i, z = -5
        Repeater3D {
            model: mainWindow.bodyCount
            SoftBody {
                id: cubeBody
                position: Qt.vector3d(mainWindow.rowStartX + index * mainWindow.rowStep,
                                      mainWindow.rowY,
                                      mainWindow.cubeRowZ)
                objectLayer: moving
                restitution: 0.1 * index
                allowSleeping: false
                sharedSettings: ExampleSoftBodyCubeSettings {}

                Model {
                    geometry: ExampleSoftBodyCubeMeshGeometry {
                        softBody: cubeBody
                    }
                    materials: PrincipledMaterial {
                        cullMode: PrincipledMaterial.NoCulling
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[(cubeBody.bodyID + 1) % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }
    }

    SettingsView {}
}
