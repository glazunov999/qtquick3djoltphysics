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
                camera.lookAt(Qt.vector3d(0, 10, 0))
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

        Component {
            id: compoundBodyComponent
            Body {
                id: body
                shape: MutableCompoundShape {
                    shapes: [
                        // Same footprint as Jolt's 2x1x1 box with Z=90° (axis-aligned for non-uniform scale).
                        BoxShape {
                            extents: Qt.vector3d(1, 2, 1)
                            position: Qt.vector3d(-5.5, -0.5, 0)
                        },
                        BoxShape {
                            extents: Qt.vector3d(1, 1, 5)
                            position: Qt.vector3d(5.5, 0, 2)
                        },
                        BoxShape {
                            extents: Qt.vector3d(10, 1, 1)
                            position: Qt.vector3d(0, 0, 0)
                        }
                    ]
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    source: "#Cube"
                    position: Qt.vector3d(-5.5, -0.5, 0)
                    scale: Qt.vector3d(0.01, 0.02, 0.01)
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                        cullMode: body.scale.x < 0 ? Material.FrontFaceCulling : Material.BackFaceCulling
                    }
                }
                Model {
                    source: "#Cube"
                    position: Qt.vector3d(5.5, 0, 2)
                    scale: Qt.vector3d(0.01, 0.01, 0.05)
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                        cullMode: body.scale.x < 0 ? Material.FrontFaceCulling : Material.BackFaceCulling
                    }
                }
                Model {
                    source: "#Cube"
                    position: Qt.vector3d(0, 0, 0)
                    scale: Qt.vector3d(0.1, 0.01, 0.01)
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                        cullMode: body.scale.x < 0 ? Material.FrontFaceCulling : Material.BackFaceCulling
                    }
                }
            }
        }

        Component.onCompleted: {
            compoundBodyComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(-40, 10, 0) })
            compoundBodyComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(-20, 10, 0), scale: Qt.vector3d(0.25, 0.25, 0.25) })
            compoundBodyComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(0, 10, 0), scale: Qt.vector3d(0.25, 0.5, 1.5) })
            compoundBodyComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(20, 10, 0), scale: Qt.vector3d(-0.25, 0.5, -1.5) })
            compoundBodyComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(40, 10, 0), scale: Qt.vector3d(-0.25, 0.5, 1.5) })
        }
    }

    SettingsView {}
}
