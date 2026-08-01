import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
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

        Body {
            id: body1
            shape: TaperedCapsuleShape {
                height: 4
                topDiameter: 2
                bottomDiameter: 6
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 10, 0)
            Model {
                geometry: TaperedCapsuleGeometry {
                    height: 4
                    topDiameter: 2
                    bottomDiameter: 6
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: colors[body1.bodyID % colors.length]
                }
            }
        }

        Body {
            id: body2
            shape: TaperedCapsuleShape {
                height: 4
                topDiameter: 6
                bottomDiameter: 2
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(10, 10, 0)
            Model {
                geometry: TaperedCapsuleGeometry {
                    height: 4
                    topDiameter: 6
                    bottomDiameter: 2
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: colors[body2.bodyID % colors.length]
                }
            }
        }

        Body {
            id: body3
            shape: TaperedCapsuleShape {
                height: 4
                topDiameter: 2
                bottomDiameter: 6
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(20, 10, 0)
            eulerRotation.x: 90
            Model {
                geometry: TaperedCapsuleGeometry {
                    height: 4
                    topDiameter: 2
                    bottomDiameter: 6
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: colors[body3.bodyID % colors.length]
                }
            }
        }

        Component {
            id: taperedCapsuleComponent
            Body {
                id: taperedCapsuleBody
                shape: TaperedCapsuleShape {
                    height: 10
                    topDiameter: 1
                    bottomDiameter: 2
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    geometry: TaperedCapsuleGeometry {
                        height: 10
                        topDiameter: 1
                        bottomDiameter: 2
                    }
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        metalness: 0.0
                        roughness: 0.1
                        baseColor: colors[taperedCapsuleBody.bodyID % colors.length]
                    }
                }
            }
        }

        Component.onCompleted: {
            for (let i = 0; i < 10; ++i) {
                for (let j = 0; j < 2; ++j) {
                    let position
                    let eulerRotation = Qt.vector3d(0, 0, 0)
                    if (i & 1) {
                        position = Qt.vector3d(-4 + 8 * j, 2 + 3 * i, -20)
                        eulerRotation.x = j ? 270 : 90
                    } else {
                        position = Qt.vector3d(0, 2 + 3 * i, -20 - 4 + 8 * j)
                        eulerRotation.z = j ? 270 : 90
                    }
                    taperedCapsuleComponent.createObject(viewport.scene, {
                        position: position,
                        eulerRotation: eulerRotation
                    })
                }
            }
        }
    }

    SettingsView {}
}
