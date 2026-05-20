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

    // Mirrors Jolt Samples/Tests/Constraints/RackAndPinionConstraintTest.cpp (gallery scale).
    readonly property real pinionRadius: 2
    readonly property real pinionHalfWidth: 0.2
    readonly property real rackLength: 40
    readonly property real rackHalfHeight: 0.4
    readonly property real rackHalfWidth: 0.2
    readonly property real toothGap: 0.2
    readonly property int pinionNumTeeth: 100
    readonly property int rackNumTeeth: Math.round(
        rackLength * pinionNumTeeth / (2 * Math.PI * pinionRadius))
    readonly property real rackPinionRatio: RackAndPinionConstraint.ratioFromTeeth(
        rackNumTeeth, rackLength, pinionNumTeeth)

    readonly property var anchor: Qt.vector3d(0, 10, 0)
    readonly property var pinionPosition: anchor
    readonly property var rackPosition: Qt.vector3d(
        anchor.x,
        anchor.y - pinionRadius - rackHalfHeight - toothGap,
        anchor.z)

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
            position: Qt.vector3d(25, 18, 30)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: camera.lookAt(anchor)
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
            shape: BoxShape { extents: Qt.vector3d(200, 2, 200) }
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
            id: rackPinionFilter
            numSubGroups: 2
            Component.onCompleted: disableCollision(0, 1)
        }

        Body {
            id: mount
            position: anchor
            shape: EmptyShape {}
            objectLayer: nonMoving
            motionType: Body.Static
        }

        // Pinion (gear), vertical disc — Jolt: rotation 90° around X, hinge axis Z
        Body {
            id: pinion
            position: pinionPosition
            eulerRotation.x: 90
            shape: CylinderShape {
                height: pinionHalfWidth * 2
                diameter: pinionRadius * 2
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: rackPinionFilter
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
                    pinionRadius * 2 / 100,
                    pinionHalfWidth * 2 / 100,
                    pinionRadius * 2 / 100)
                materials: PrincipledMaterial {
                    baseColor: colors[pinion.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }

            function applySpin() {
                if (bodyID !== 0)
                    pinion.setAngularVelocity(Qt.vector3d(0, 0, 6))
            }

            Component.onCompleted: applySpin()
            onBodyIDChanged: applySpin()
        }

        // Rack (slider along X)
        Body {
            id: rack
            position: rackPosition
            shape: BoxShape {
                extents: Qt.vector3d(rackLength * 0.5, rackHalfWidth, rackHalfHeight)
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: rackPinionFilter
                groupId: 0
                subGroupId: 1
            }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 4
            Model {
                source: "#Cube"
                scale: Qt.vector3d(
                    rackLength / 100,
                    rackHalfWidth * 2 / 100,
                    rackHalfHeight * 2 / 100)
                materials: PrincipledMaterial {
                    baseColor: colors[rack.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        HingeConstraint {
            id: pinionHinge
            body1: mount
            body2: pinion
            point1: anchor
            point2: anchor
            hingeAxis1: Qt.vector3d(0, 0, 1)
            hingeAxis2: Qt.vector3d(0, 0, 1)
            normalAxis1: Qt.vector3d(1, 0, 0)
            normalAxis2: Qt.vector3d(1, 0, 0)
        }

        SliderConstraint {
            id: rackSlider
            body1: mount
            body2: rack
            point1: anchor
            point2: anchor
            sliderAxis1: Qt.vector3d(1, 0, 0)
            sliderAxis2: Qt.vector3d(1, 0, 0)
            normalAxis1: Qt.vector3d(0, 0, 1)
            normalAxis2: Qt.vector3d(0, 0, 1)
            limitsMin: -rackLength * 0.5
            limitsMax: rackLength * 0.5
        }

        RackAndPinionConstraint {
            body1: pinion
            body2: rack
            hingeAxis: Qt.vector3d(0, 0, 1)
            sliderAxis: Qt.vector3d(1, 0, 0)
            ratio: rackPinionRatio
            pinionConstraint: pinionHinge
            rackConstraint: rackSlider
        }
    }

    SettingsView {}
}
