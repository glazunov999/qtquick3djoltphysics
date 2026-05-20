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

    // Jolt half extents → QML full extents (BoxShape divides by 2 for Jolt)
    readonly property real lightSize: 0.2   // Vec3::sReplicate(0.1f)
    readonly property real heavySize: 10    // Vec3::sReplicate(5.0f)
    readonly property real boxSize: 2       // Vec3::sReplicate(1.0f)
    readonly property real lightScale: lightSize / 100
    readonly property real heavyScale: heavySize / 100
    readonly property real boxScale: boxSize / 100

    // Jolt chain: box_size = 4 (full extent), straight segment along +X
    readonly property real chainBoxSize: 4
    readonly property real chainScale: chainBoxSize / 100
    readonly property int chainLength: 10
    readonly property real chainY: 25
    readonly property var chainSliderAxis: {
        const rad = -10 * Math.PI / 180
        return Qt.vector3d(Math.cos(rad), Math.sin(rad), 0)
    }

    function normalForSliderAxis(axis) {
        const len = Math.sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z) || 1
        const a = Qt.vector3d(axis.x / len, axis.y / len, axis.z / len)
        let ref = Qt.vector3d(0, 1, 0)
        if (Math.abs(a.y) > 0.99)
            ref = Qt.vector3d(1, 0, 0)
        const n = Qt.vector3d(
            a.y * ref.z - a.z * ref.y,
            a.z * ref.x - a.x * ref.z,
            a.x * ref.y - a.y * ref.x)
        const nlen = Math.sqrt(n.x * n.x + n.y * n.y + n.z * n.z)
        return Qt.vector3d(n.x / nlen, n.y / nlen, n.z / nlen)
    }

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
            position: Qt.vector3d(12, 10, 18)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: camera.lookAt(Qt.vector3d(20, 16, 0))
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
            id: chainFilter
            numSubGroups: chainLength
            Component.onCompleted: {
                for (let i = 0; i < chainLength - 1; ++i)
                    disableCollision(i, i + 1)
            }
        }

        Body {
            id: chainTop
            position: Qt.vector3d(0, chainY, 0)
            shape: BoxShape {
                extents: Qt.vector3d(chainBoxSize, chainBoxSize, chainBoxSize)
            }
            objectLayer: nonMoving
            motionType: Body.Static
            collisionGroup: CollisionGroup {
                groupFilterTable: chainFilter
                groupId: 0
                subGroupId: 0
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(chainScale, chainScale, chainScale)
                materials: PrincipledMaterial {
                    baseColor: "lightgray"
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Component {
            id: chainLinkComponent
            Body {
                id: chainLink
                property int segmentIndex: 1
                property vector3d linkPosition: Qt.vector3d(0, chainY, 0)

                position: linkPosition
                shape: BoxShape {
                    extents: Qt.vector3d(chainBoxSize, chainBoxSize, chainBoxSize)
                }
                objectLayer: moving
                motionType: Body.Dynamic
                collisionGroup: CollisionGroup {
                    groupFilterTable: chainFilter
                    groupId: 0
                    subGroupId: segmentIndex
                }
                Model {
                    source: "#Cube"
                    scale: Qt.vector3d(chainScale, chainScale, chainScale)
                    materials: PrincipledMaterial {
                        baseColor: colors[chainLink.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        Component {
            id: chainSliderComponent
            SliderConstraint {
                autoDetectPoint: true
                limitsMin: -5
                limitsMax: 10
            }
        }

        function initChain() {
            let prev = chainTop
            let position = Qt.vector3d(chainTop.position.x, chainTop.position.y, chainTop.position.z)
            const sliderAxis = chainSliderAxis
            const normalAxis = normalForSliderAxis(sliderAxis)

            for (let i = 1; i < chainLength; ++i) {
                position = position.plus(Qt.vector3d(chainBoxSize, 0, 0))
                const link = chainLinkComponent.createObject(viewport.scene, {
                    segmentIndex: i,
                    linkPosition: position
                })
                chainSliderComponent.createObject(viewport.scene, {
                    body1: prev,
                    body2: link,
                    sliderAxis1: sliderAxis,
                    sliderAxis2: sliderAxis,
                    normalAxis1: normalAxis,
                    normalAxis2: normalAxis
                })
                prev = link
            }
        }

        Component.onCompleted: Qt.callLater(initChain)

        GroupFilterTable {
            id: tripleFilter
            numSubGroups: 3
            Component.onCompleted: {
                disableCollision(0, 1)
                disableCollision(1, 2)
            }
        }

        // Two light bodies attached to a heavy body (Jolt x = -5)
        Body {
            id: light1
            position: Qt.vector3d(-5, 7, -5.2)
            shape: BoxShape {
                extents: Qt.vector3d(lightSize, lightSize, lightSize)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: tripleFilter
                groupId: 2
                subGroupId: 0
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(lightScale, lightScale, lightScale)
                materials: PrincipledMaterial {
                    baseColor: colors[light1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: heavy
            position: Qt.vector3d(-5, 7, 0)
            shape: BoxShape {
                extents: Qt.vector3d(heavySize, heavySize, heavySize)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: tripleFilter
                groupId: 2
                subGroupId: 1
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(heavyScale, heavyScale, heavyScale)
                materials: PrincipledMaterial {
                    baseColor: colors[heavy.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: light2
            position: Qt.vector3d(-5, 7, 5.2)
            shape: BoxShape {
                extents: Qt.vector3d(lightSize, lightSize, lightSize)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: tripleFilter
                groupId: 2
                subGroupId: 2
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(lightScale, lightScale, lightScale)
                materials: PrincipledMaterial {
                    baseColor: colors[light2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        // body1 lighter than body2 — rotation constraint less accurate (Jolt comment)
        SliderConstraint {
            body1: light1
            body2: heavy
            autoDetectPoint: true
            sliderAxis1: Qt.vector3d(0, 0, 1)
            sliderAxis2: Qt.vector3d(0, 0, 1)
            normalAxis1: Qt.vector3d(0, 1, 0)
            normalAxis2: Qt.vector3d(0, 1, 0)
            limitsMin: 0
            limitsMax: 1
        }

        // heavy as body1 — works as intended
        SliderConstraint {
            body1: heavy
            body2: light2
            autoDetectPoint: true
            sliderAxis1: Qt.vector3d(0, 0, 1)
            sliderAxis2: Qt.vector3d(0, 0, 1)
            normalAxis1: Qt.vector3d(0, 1, 0)
            normalAxis2: Qt.vector3d(0, 1, 0)
            limitsMin: 0
            limitsMax: 1
        }

        GroupFilterTable {
            id: vertFilter
            numSubGroups: 2
            Component.onCompleted: disableCollision(0, 1)
        }

        // Two bodies vertically stacked with a slider constraint (Jolt x = 5)
        Body {
            id: vert1
            position: Qt.vector3d(5, 9, 0)
            shape: BoxShape { extents: Qt.vector3d(boxSize, boxSize, boxSize) }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: vertFilter
                groupId: 3
                subGroupId: 0
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(boxScale, boxScale, boxScale)
                materials: PrincipledMaterial {
                    baseColor: colors[vert1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: vert2
            position: Qt.vector3d(5, 3, 0)
            shape: BoxShape { extents: Qt.vector3d(boxSize, boxSize, boxSize) }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: vertFilter
                groupId: 3
                subGroupId: 1
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(boxScale, boxScale, boxScale)
                materials: PrincipledMaterial {
                    baseColor: colors[vert2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        SliderConstraint {
            body1: vert1
            body2: vert2
            autoDetectPoint: true
            sliderAxis1: Qt.vector3d(0, 1, 0)
            sliderAxis2: Qt.vector3d(0, 1, 0)
            normalAxis1: Qt.vector3d(1, 0, 0)
            normalAxis2: Qt.vector3d(1, 0, 0)
            limitsMin: 0
            limitsMax: 2
        }

        // Two bodies vertically stacked with a slider constraint using soft limits (Jolt x = 10)
        Body {
            id: softVert1
            position: Qt.vector3d(10, 9, 0)
            shape: BoxShape { extents: Qt.vector3d(boxSize, boxSize, boxSize) }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: vertFilter
                groupId: 4
                subGroupId: 0
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(boxScale, boxScale, boxScale)
                materials: PrincipledMaterial {
                    baseColor: colors[softVert1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: softVert2
            position: Qt.vector3d(10, 3, 0)
            shape: BoxShape { extents: Qt.vector3d(boxSize, boxSize, boxSize) }
            objectLayer: moving
            motionType: Body.Dynamic
            collisionGroup: CollisionGroup {
                groupFilterTable: vertFilter
                groupId: 4
                subGroupId: 1
            }
            Model {
                source: "#Cube"
                scale: Qt.vector3d(boxScale, boxScale, boxScale)
                materials: PrincipledMaterial {
                    baseColor: colors[softVert2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        SliderConstraint {
            body1: softVert1
            body2: softVert2
            autoDetectPoint: true
            sliderAxis1: Qt.vector3d(0, 1, 0)
            sliderAxis2: Qt.vector3d(0, 1, 0)
            normalAxis1: Qt.vector3d(1, 0, 0)
            normalAxis2: Qt.vector3d(1, 0, 0)
            limitsMin: 0
            limitsMax: 2
            limitsSpringSettings: SpringSettings {
                frequency: 1
                damping: 0.5
            }
        }

    }

    SettingsView {}
}
