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

    // Jolt Samples/Tests/SoftBody/SoftBodyContactListenerTest
    readonly property real cycleDuration: 2.5
    readonly property var cycleNames: [
        "Accept contact",
        "Sphere 10x mass",
        "Cloth 10x mass",
        "Sphere infinite mass",
        "Cloth infinite mass",
        "Sensor contact",
        "Reject contact",
        "Kinematic Sphere",
        "Kinematic Sphere, cloth infinite mass",
        "Kinematic sphere, sensor contact"
    ]

    property int cycle: 0
    property real cycleTime: 0

    ExampleSoftBodyContactListener {
        id: contactListener
        cycle: mainWindow.cycle
    }

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: AppSettings.playPhysics
        time: timer.elapsedTime * 1000
        gravity: Qt.vector3d(0, -AppSettings.gravity, 0)
        numThreads: AppSettings.maxConcurrentJobs
        collisionSteps: AppSettings.collisionSteps
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        softBodyContactListener: contactListener
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

        onBeforeFrameDone: (deltaTime) => prePhysicsUpdate(deltaTime)
    }

    FrameAnimation {
        id: timer
        running: AppSettings.playPhysics
    }

    function prePhysicsUpdate(deltaTime) {
        cycleTime += deltaTime
        if (cycleTime < cycleDuration)
            return

        cycleTime = 0
        cycle = (cycle + 1) % cycleNames.length
        cycleLoader.active = false
        cycleLoader.active = true
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
            position: Qt.vector3d(12, 8, 18)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(0, 5, 0))
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

        Loader3D {
            id: cycleLoader
            position: Qt.vector3d(0, 0, 0)
            active: true
            sourceComponent: cycleScene
        }
    }

    Component {
        id: cycleScene
        Node {
            SoftBody {
                id: clothBody
                position: Qt.vector3d(0, 5, 0)
                eulerRotation.y: 45
                objectLayer: moving
                allowSleeping: false
                updatePosition: false
                makeRotationIdentity: false
                sharedSettings: ExampleSoftBodyClothSettings {
                    gridSizeX: 15
                    gridSizeZ: 15
                    gridSpacing: 0.75
                    pinFourCorners: true
                }

                Model {
                    geometry: ExampleSoftBodyClothMeshGeometry {
                        softBody: clothBody
                    }
                    materials: PrincipledMaterial {
                        cullMode: PrincipledMaterial.NoCulling
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[(clothBody.bodyID + 1) % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }

            Body {
                id: sphereBody
                position: Qt.vector3d(0, 7, 0)
                objectLayer: moving
                motionType: mainWindow.cycle > 6 ? Body.Kinematic : Body.Dynamic
                overrideMassProperties: Body.CalculateInertia
                mass: 100
                allowSleeping: false
                shape: SphereShape {
                    diameter: 2
                }

                Model {
                    source: "#Sphere"
                    materials: PrincipledMaterial {
                        baseColor: colors[(sphereBody.bodyID + 1) % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                    scale: Qt.vector3d(0.02, 0.02, 0.02)
                }

                Component.onCompleted: {
                    if (mainWindow.cycle > 6)
                        sphereBody.setLinearVelocity(Qt.vector3d(0, -2.5, 0))
                }
            }
        }
    }

    Column {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 16
        spacing: 6

        Text {
            color: "#f0f0f0"
            font.pointSize: AppSettings.fontSizeLarge
            text: cycleNames[cycle]
        }

        Text {
            color: "#c0c0c0"
            font.pointSize: AppSettings.fontSizeSmall
            text: qsTr("Vertices in contact: %1").arg(contactListener.contactVertexCount)
        }
    }

    SettingsView {}
}
