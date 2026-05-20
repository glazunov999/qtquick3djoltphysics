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
            position: Qt.vector3d(35, 25, 35)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(0, 8, -15))
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

        // Mirrors Jolt Samples/Tests/Constraints/PulleyConstraintTest.cpp — four rows along -Z.

        // Variation 0: default limits (min 0, max -1 → auto at creation) — rope-like, can shorten
        Body {
            id: v0a
            position: Qt.vector3d(-10, 10, 0)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v0a.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        Body {
            id: v0b
            position: Qt.vector3d(10, 10, 0)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v0b.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        PulleyConstraint {
            body1: v0a
            body2: v0b
            bodyPoint1: Qt.vector3d(-10, 10.5, 0)
            fixedPoint1: Qt.vector3d(-10, 20.5, 0)
            bodyPoint2: Qt.vector3d(10, 10.5, 0)
            fixedPoint2: Qt.vector3d(10, 20.5, 0)
        }

        // Variation 1: fixed length (min = max = -1 → both set from current length at creation)
        Body {
            id: v1a
            position: Qt.vector3d(-10, 10, -10)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v1a.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        Body {
            id: v1b
            position: Qt.vector3d(10, 10, -10)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v1b.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        PulleyConstraint {
            body1: v1a
            body2: v1b
            bodyPoint1: Qt.vector3d(-10, 10.5, -10)
            fixedPoint1: Qt.vector3d(-10, 20.5, -10)
            bodyPoint2: Qt.vector3d(10, 10.5, -10)
            fixedPoint2: Qt.vector3d(10, 20.5, -10)
            minLength: -1
            maxLength: -1
        }

        // Variation 2: min / max length range
        Body {
            id: v2a
            position: Qt.vector3d(-10, 10, -20)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v2a.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        Body {
            id: v2b
            position: Qt.vector3d(10, 10, -20)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v2b.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        PulleyConstraint {
            body1: v2a
            body2: v2b
            bodyPoint1: Qt.vector3d(-10, 10.5, -20)
            fixedPoint1: Qt.vector3d(-10, 20.5, -20)
            bodyPoint2: Qt.vector3d(10, 10.5, -20)
            fixedPoint2: Qt.vector3d(10, 20.5, -20)
            minLength: 18
            maxLength: 22
        }

        // Variation 3: ratio (block and tackle style)
        Body {
            id: v3a
            position: Qt.vector3d(-10, 10, -30)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v3a.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        Body {
            id: v3b
            position: Qt.vector3d(10, 10, -30)
            shape: BoxShape { extents: Qt.vector3d(1, 1, 1) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.01, 0.01)
                materials: PrincipledMaterial {
                    baseColor: colors[v3b.bodyID % colors.length]
                    metalness: 0.2
                    roughness: 0.6
                }
            }
        }
        PulleyConstraint {
            body1: v3a
            body2: v3b
            bodyPoint1: Qt.vector3d(-10, 10.5, -30)
            fixedPoint1: Qt.vector3d(-10, 20.5, -30)
            bodyPoint2: Qt.vector3d(10, 10.5, -30)
            fixedPoint2: Qt.vector3d(10, 20.5, -30)
            ratio: 4
        }
    }

    SettingsView {}
}
