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

    readonly property real cylinderHeight: 2
    readonly property real cylinderDiameter: 0.2
    readonly property vector3d cylinderModelScale: Qt.vector3d(
        cylinderDiameter / 100, cylinderHeight / 100, cylinderDiameter / 100)

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
                camera.lookAt(Qt.vector3d(0, 5, -5))
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
            friction: 1.0
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
            id: topCylinder
            shape: CylinderShape {
                height: cylinderHeight
                diameter: cylinderDiameter
            }
            scale: Qt.vector3d(2, 1, 2)
            offsetCenterOfMass: Qt.vector3d(0, 1, 0)
            friction: 1.0
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(-5, 5, 0)
            eulerRotation.z: 72
            Model {
                source: "#Cylinder"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[topCylinder.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: cylinderModelScale
            }
        }

        Body {
            id: centerCylinder
            shape: CylinderShape {
                height: cylinderHeight
                diameter: cylinderDiameter
            }
            scale: Qt.vector3d(2, 1, 2)
            friction: 1.0
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 5, 0)
            eulerRotation.z: 72
            Model {
                source: "#Cylinder"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[centerCylinder.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: cylinderModelScale
            }
        }

        Body {
            id: bottomCylinder
            shape: CylinderShape {
                height: cylinderHeight
                diameter: cylinderDiameter
            }
            scale: Qt.vector3d(2, 1, 2)
            offsetCenterOfMass: Qt.vector3d(0, -1, 0)
            friction: 1.0
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(5, 5, 0)
            eulerRotation.z: 72
            Model {
                source: "#Cylinder"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[bottomCylinder.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: cylinderModelScale
            }
        }

        Body {
            id: preScaledSphere
            shape: SphereShape {
                diameter: 2
            }
            scale: Qt.vector3d(2, 2, 2)
            offsetCenterOfMass: Qt.vector3d(0, 0, 5)
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 5, -15)
            Model {
                source: "#Sphere"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[preScaledSphere.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.02, 0.02, 0.02)
            }
        }

        Body {
            id: postScaledSphere
            shape: SphereShape {
                diameter: 2
            }
            scale: Qt.vector3d(2, 2, 2)
            offsetCenterOfMass: Qt.vector3d(0, 0, 10)
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(5, 5, -15)
            Model {
                source: "#Sphere"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[postScaledSphere.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.02, 0.02, 0.02)
            }
        }
    }

    SettingsView {}
}
