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

    // Mirrors Jolt Samples/Tests/Constraints/PathConstraintTest.cpp

    function buildSpiralPositions() {
        const positions = []
        for (let a = -0.1 * Math.PI; a < 4.0 * Math.PI; a += 0.1 * Math.PI)
            positions.push(Qt.vector3d(5 * Math.cos(a), -a, 5 * Math.sin(a)))
        return positions
    }

    function buildCirclePositions() {
        const positions = []
        for (let i = -1; i < 11; ++i) {
            const a = 2.0 * Math.PI * i / 10.0
            positions.push(Qt.vector3d(5 * Math.cos(a), 0, 5 * Math.sin(a)))
        }
        return positions
    }

    function hermitePathPoints(positions) {
        const points = []
        for (let i = 1; i < positions.length - 1; ++i) {
            const prev = positions[i - 1]
            const next = positions[i + 1]
            points.push({
                position: positions[i],
                tangent: Qt.vector3d(0.5 * (next.x - prev.x),
                                      0.5 * (next.y - prev.y),
                                      0.5 * (next.z - prev.z)),
                normal: Qt.vector3d(0, 1, 0)
            })
        }
        return points
    }

    readonly property var spiralHermitePoints: hermitePathPoints(buildSpiralPositions())
    readonly property var circleHermitePoints: hermitePathPoints(buildCirclePositions())
    readonly property var circlePathRotation: Qt.quaternion(
        Math.cos(-0.05 * Math.PI), Math.sin(-0.05 * Math.PI), 0, 0)

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
            position: Qt.vector3d(0, 25, 45)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: camera.lookAt(Qt.vector3d(0, 5, 0))
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

        // Spiral path (left, Jolt variation 0)
        Body {
            id: spiralBase
            position: Qt.vector3d(-10, 1, 0)
            shape: BoxShape { extents: Qt.vector3d(5, 0.5, 5) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 50
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.1, 0.01, 0.1)
                materials: PrincipledMaterial {
                    baseColor: colors[spiralBase.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: spiralRider
            position: Qt.vector3d(-5, 15, 0)
            allowSleeping: false
            shape: BoxShape { extents: Qt.vector3d(0.5, 1, 2) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.02, 0.04)
                materials: PrincipledMaterial {
                    baseColor: colors[spiralRider.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        HermitePath {
            id: spiralPath
            Component.onCompleted: {
                for (let i = 0; i < spiralHermitePoints.length; ++i) {
                    const pt = spiralHermitePoints[i]
                    addPoint(pt.position, pt.tangent, pt.normal)
                }
            }
        }

        PathConstraint {
            body1: spiralBase
            body2: spiralRider
            path: spiralPath
            pathPosition: Qt.vector3d(0, 15, 0)
            rotationConstraintType: PathConstraint.Free
        }

        // Circular looping path (right, Jolt variation 1)
        Body {
            id: circleBase
            position: Qt.vector3d(10, 1, 0)
            shape: BoxShape { extents: Qt.vector3d(5, 0.5, 5) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 50
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.1, 0.01, 0.1)
                materials: PrincipledMaterial {
                    baseColor: colors[circleBase.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: circleRider
            position: Qt.vector3d(15, 5, 0)
            allowSleeping: false
            shape: BoxShape { extents: Qt.vector3d(0.5, 1, 2) }
            objectLayer: moving
            motionType: Body.Dynamic
            overrideMassProperties: Body.CalculateInertia
            mass: 2
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.01, 0.02, 0.04)
                materials: PrincipledMaterial {
                    baseColor: colors[circleRider.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        HermitePath {
            id: circlePath
            isLooping: true
            Component.onCompleted: {
                for (let i = 0; i < circleHermitePoints.length; ++i) {
                    const pt = circleHermitePoints[i]
                    addPoint(pt.position, pt.tangent, pt.normal)
                }
            }
        }

        PathConstraint {
            body1: circleBase
            body2: circleRider
            path: circlePath
            pathPosition: Qt.vector3d(0, 5, 0)
            pathRotation: circlePathRotation
            rotationConstraintType: PathConstraint.Free
        }
    }

    SettingsView {}
}
