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

    readonly property real halfCylinderHeight: 2.5
    readonly property real capsuleHeight: halfCylinderHeight * 2
    readonly property real capsuleDiameter: 2

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
            position: Qt.vector3d(30, 25, 30)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(10, 20, 5))
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
            id: groupFilterComponent
            GroupFilterTable {
                property int chainLength: 5
                numSubGroups: chainLength
                Component.onCompleted: {
                    for (let i = 0; i < chainLength - 1; ++i)
                        disableCollision(i, i + 1)
                }
            }
        }

        Component {
            id: bodyComponent
            Body {
                id: body
                property int index: 0
                property int groupId: 0
                property var groupFilter
                shape: CapsuleShape {
                    height: capsuleHeight
                    diameter: capsuleDiameter
                }
                collisionGroup: CollisionGroup {
                    groupFilterTable: groupFilter
                    groupId: groupId
                    subGroupId: index
                }
                objectLayer: index === 0 ? nonMoving : moving
                motionType: index === 0 ? Body.Static : Body.Dynamic
                allowSleeping: false
                eulerRotation.z: 90
                Model {
                    eulerRotation.z: -90
                    geometry: CapsuleGeometry {
                        height: capsuleHeight
                        diameter: capsuleDiameter
                    }
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        Component {
            id: constraintComponent
            ConeConstraint {}
        }

        function buildChain(chainIndex, halfConeAngle) {
            let groupFilter = groupFilterComponent.createObject(viewport.scene, { chainLength: 5 })
            let prev = null
            let baseRotation = Quaternion.fromAxisAndAngle(0, 0, 1, 90)
            let position = Qt.vector3d(0, 20, 10 * chainIndex)

            for (let i = 0; i < 5; ++i) {
                position = position.plus(Qt.vector3d(2.0 * halfCylinderHeight, 0, 0))
                let body = bodyComponent.createObject(viewport.scene, {
                    index: i,
                    groupId: chainIndex,
                    position: position,
                    rotation: Quaternion.fromAxisAndAngle(1, 0, 0, 45 * i).times(baseRotation),
                    groupFilter: groupFilter
                })

                if (prev !== null) {
                    let joint = position.plus(Qt.vector3d(-halfCylinderHeight, 0, 0))
                    constraintComponent.createObject(viewport.scene, {
                        point1: joint,
                        point2: joint,
                        body1: prev,
                        body2: body,
                        twistAxis1: Qt.vector3d(1, 0, 0),
                        twistAxis2: Qt.vector3d(1, 0, 0),
                        halfConeAngle: halfConeAngle
                    })
                }

                prev = body
            }
        }

        Component.onCompleted: {
            buildChain(0, 0)
            buildChain(1, 20)
        }
    }

    SettingsView {}
}
