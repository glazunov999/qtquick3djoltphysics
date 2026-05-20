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

    // Proportions from Jolt GearConstraintTest; sizes match gallery (see SimpleTest).
    readonly property real gearHalfWidth: 0.2
    readonly property real gear1Radius: 2
    readonly property real gear2Radius: 8
    readonly property int gear1NumTeeth: 100
    readonly property int gear2NumTeeth: gear1NumTeeth * gear2Radius / gear1Radius
    readonly property real gearRatio: gear2NumTeeth / gear1NumTeeth

    readonly property real gearRowY: 12
    readonly property real gearRowZ: 0
    readonly property var gear1Position: Qt.vector3d(0, gearRowY, gearRowZ)
    readonly property var gear2Position: Qt.vector3d(
        gear1Position.x + gear1Radius + gear2Radius,
        gearRowY,
        gearRowZ)

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
            position: Qt.vector3d(gear2Position.x + 15, gearRowY + 8, 25)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(
                    (gear1Position.x + gear2Position.x) * 0.5,
                    gearRowY,
                    gearRowZ))
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

        GroupFilterTable {
            id: gearGroupFilter
            numSubGroups: 2
            Component.onCompleted: disableCollision(0, 1)
        }

        // Static mounts (Jolt: Body::sFixedToWorld) — no collision shape
        Body {
            id: mount1
            position: gear1Position
            shape: EmptyShape {}
            objectLayer: nonMoving
            motionType: Body.Static
        }

        Body {
            id: mount2
            position: gear2Position
            shape: EmptyShape {}
            objectLayer: nonMoving
            motionType: Body.Static
        }

        // Vertical discs (Jolt: body rotated 90° X, hinge axis Z), meshing along +X
        Body {
            id: gear1
            position: gear1Position
            eulerRotation.x: 90
            shape: CylinderShape {
                height: gearHalfWidth * 2
                diameter: gear1Radius * 2
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: gearGroupFilter
                groupId: 0
                subGroupId: 0
            }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cylinder"
                scale: Qt.vector3d(
                    gear1Radius * 2 / 100,
                    gearHalfWidth * 2 / 100,
                    gear1Radius * 2 / 100)
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[gear1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: gear2
            position: gear2Position
            eulerRotation.x: 90
            shape: CylinderShape {
                height: gearHalfWidth * 2
                diameter: gear2Radius * 2
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: gearGroupFilter
                groupId: 0
                subGroupId: 1
            }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 8
            Model {
                source: "#Cylinder"
                scale: Qt.vector3d(
                    gear2Radius * 2 / 100,
                    gearHalfWidth * 2 / 100,
                    gear2Radius * 2 / 100)
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[gear2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }

            function applySpin() {
                if (bodyID !== 0)
                    gear2.setAngularVelocity(Qt.vector3d(0, 0, 3))
            }

            Component.onCompleted: applySpin()
            onBodyIDChanged: applySpin()
        }

        HingeConstraint {
            id: hinge1
            body1: mount1
            body2: gear1
            point1: gear1Position
            point2: gear1Position
            hingeAxis1: Qt.vector3d(0, 0, 1)
            hingeAxis2: Qt.vector3d(0, 0, 1)
            normalAxis1: Qt.vector3d(1, 0, 0)
            normalAxis2: Qt.vector3d(1, 0, 0)
        }

        HingeConstraint {
            id: hinge2
            body1: mount2
            body2: gear2
            point1: gear2Position
            point2: gear2Position
            hingeAxis1: Qt.vector3d(0, 0, 1)
            hingeAxis2: Qt.vector3d(0, 0, 1)
            normalAxis1: Qt.vector3d(1, 0, 0)
            normalAxis2: Qt.vector3d(1, 0, 0)
        }

        GearConstraint {
            body1: gear1
            body2: gear2
            hingeAxis1: Qt.vector3d(0, 0, 1)
            hingeAxis2: Qt.vector3d(0, 0, 1)
            ratio: gearRatio
            gear1Constraint: hinge1
            gear2Constraint: hinge2
        }
    }

    SettingsView {}
}
