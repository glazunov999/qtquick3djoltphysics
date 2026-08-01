import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import Example

import ".."

Item {
    id: mainWindow

    anchors.fill: parent

    readonly property int nonMoving: 0
    readonly property int moving: 1

    readonly property var scaleVariants: [
        { x: -40, scale: Qt.vector3d(1, 1, 1) },
        { x: -20, scale: Qt.vector3d(0.25, 0.25, 0.25) },
        { x: 0, scale: Qt.vector3d(0.25, 0.5, 1.5) },
        { x: 20, scale: Qt.vector3d(-0.25, 0.5, -1.5) },
        { x: 40, scale: Qt.vector3d(-0.25, 0.5, 1.5) }
    ]

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: AppSettings.playPhysics
        gravity: Qt.vector3d(0, -AppSettings.gravity, 0)
        numThreads: AppSettings.maxConcurrentJobs
        minimumTimestep: AppSettings.minimumTimestep
        maximumTimestep: AppSettings.maximumTimestep
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
            position: Qt.vector3d(60, 20, 60)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(0, 10, 10))
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

        ExampleBoxGeometry {
            id: boxGeometry
        }

        Repeater {
            model: scaleVariants
            delegate: Body {
                required property int index
                required property var modelData

                shape: ConvexHullShape {
                    geometry: boxGeometry
                }
                scale: modelData.scale
                objectLayer: moving
                motionType: Body.Dynamic
                position: Qt.vector3d(modelData.x, 10, 0)
                eulerRotation: Qt.vector3d(18, 36, 0)
                Model {
                    geometry: boxGeometry
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                        cullMode: modelData.scale.x < 0 ? Material.FrontFaceCulling : Material.BackFaceCulling
                    }
                }
            }
        }

        Repeater {
            model: scaleVariants
            delegate: Body {
                required property int index
                required property var modelData

                shape: ConvexHullShape {
                    geometry: boxGeometry
                }
                scale: modelData.scale
                objectLayer: moving
                motionType: Body.Dynamic
                position: Qt.vector3d(modelData.x, 10, 20)
                Model {
                    geometry: boxGeometry
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                        cullMode: modelData.scale.x < 0 ? Material.FrontFaceCulling : Material.BackFaceCulling
                    }
                }
            }
        }
    }

    SettingsView {}
}
