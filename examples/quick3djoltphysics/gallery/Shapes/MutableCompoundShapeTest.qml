import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import Example

import ".."

Item {
    id: mainWindow

    anchors.fill: parent

    readonly property int nonMoving: 0
    readonly property int moving: 1

    property var mutableBodies: []
    property int frameNumber: 0

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
        onBeforeFrameDone: deltaTime => prePhysicsUpdate(deltaTime)
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

    function randomEulerRotation() {
        return Qt.vector3d(Math.random() * 360, Math.random() * 360, Math.random() * 360)
    }

    function addShape(entry) {
        const sub = subCompoundShapeComponent.createObject(entry.compound, {
            eulerRotation: randomEulerRotation()
        })
        entry.compound.addShape(sub)

        const visual = subCompoundVisualComponent.createObject(entry.body, {
            bodyColor: colors[entry.body.bodyID % colors.length]
        })
        visual.eulerRotation = Qt.binding(function() { return sub.eulerRotation })

        entry.subShapes.push({ shape: sub, visual: visual })
    }

    function removeShape(entry, index) {
        entry.compound.removeShape(index)

        const sub = entry.subShapes[index]
        sub.visual.destroy()
        sub.shape.destroy()
        entry.subShapes.splice(index, 1)
    }

    function prePhysicsUpdate(deltaTime) {
        frameNumber++

        const deltaAngle = 20.0 * deltaTime

        for (let i = 0; i < mutableBodies.length; ++i) {
            const entry = mutableBodies[i]
            const count = entry.compound.shapeCount()

            for (let j = 0; j < count; ++j) {
                const sub = entry.compound.shapeAt(j)
                const sign = j % 2 === 0 ? 1.0 : -1.0
                sub.eulerRotation = Qt.vector3d(sub.eulerRotation.x + sign * deltaAngle,
                                                sub.eulerRotation.y,
                                                sub.eulerRotation.z)
            }
            entry.compound.modifyShapes(true)

            const roll = Math.random()
            if (roll < 0.001 && count > 1) {
                const index = Math.floor(Math.random() * count)
                removeShape(entry, index)
            } else if (roll < 0.002 && count < 10) {
                addShape(entry)
            }
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
            position: Qt.vector3d(30, 20, 30)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(0, 20, 0))
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
                extents: Qt.vector3d(200, 20, 200)
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, -10, 0)
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
                scale: Qt.vector3d(2, 0.2, 2)
            }
        }

        Component {
            id: subCompoundShapeComponent
            StaticCompoundShape {
                shapes: [
                    BoxShape {
                        position: Qt.vector3d(0, 1.5, 0)
                        eulerRotation.z: 90
                        extents: Qt.vector3d(3, 0.5, 0.4)
                    },
                    CylinderShape {
                        position: Qt.vector3d(1.5, 0, 0)
                        eulerRotation.z: 90
                        height: 3
                        diameter: 0.4
                    },
                    TaperedCapsuleShape {
                        position: Qt.vector3d(0, 0, 1.5)
                        eulerRotation.x: 90
                        height: 3
                        topDiameter: 0.5
                        bottomDiameter: 0.4
                    }
                ]
            }
        }

        Component {
            id: subCompoundVisualComponent
            Node {
                id: subVisualRoot
                property color bodyColor: "white"

                Model {
                    source: "#Cube"
                    position: Qt.vector3d(0, 1.5, 0)
                    eulerRotation.z: 90
                    scale: Qt.vector3d(3 / 100, 0.5 / 100, 0.4 / 100)
                    materials: PrincipledMaterial {
                        baseColor: subVisualRoot.bodyColor
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
                Model {
                    source: "#Cylinder"
                    position: Qt.vector3d(1.5, 0, 0)
                    eulerRotation.z: 90
                    scale: Qt.vector3d(0.4 / 100, 3 / 100, 0.4 / 100)
                    materials: PrincipledMaterial {
                        baseColor: subVisualRoot.bodyColor
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
                Model {
                    geometry: TaperedCapsuleGeometry {
                        height: 3
                        topDiameter: 0.5
                        bottomDiameter: 0.4
                    }
                    position: Qt.vector3d(0, 0, 1.5)
                    eulerRotation.x: 90
                    materials: PrincipledMaterial {
                        baseColor: subVisualRoot.bodyColor
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        Component {
            id: mutableBodyComponent
            Body {
                id: body
                property alias sub1: subCompound1
                property alias sub2: subCompound2
                property alias visual1: subVisual1
                property alias visual2: subVisual2
                property alias compound: compoundShape

                shape: MutableCompoundShape {
                    id: compoundShape
                    shapes: [
                        StaticCompoundShape {
                            id: subCompound1
                            eulerRotation: Qt.vector3d(-45, 0, 45)
                            shapes: [
                                BoxShape {
                                    position: Qt.vector3d(0, 1.5, 0)
                                    eulerRotation.z: 90
                                    extents: Qt.vector3d(3, 0.5, 0.4)
                                },
                                CylinderShape {
                                    position: Qt.vector3d(1.5, 0, 0)
                                    eulerRotation.z: 90
                                    height: 3
                                    diameter: 0.4
                                },
                                TaperedCapsuleShape {
                                    position: Qt.vector3d(0, 0, 1.5)
                                    eulerRotation.x: 90
                                    height: 3
                                    topDiameter: 0.5
                                    bottomDiameter: 0.4
                                }
                            ]
                        },
                        StaticCompoundShape {
                            id: subCompound2
                            eulerRotation: Qt.vector3d(45, 0, -45)
                            shapes: [
                                BoxShape {
                                    position: Qt.vector3d(0, 1.5, 0)
                                    eulerRotation.z: 90
                                    extents: Qt.vector3d(3, 0.5, 0.4)
                                },
                                CylinderShape {
                                    position: Qt.vector3d(1.5, 0, 0)
                                    eulerRotation.z: 90
                                    height: 3
                                    diameter: 0.4
                                },
                                TaperedCapsuleShape {
                                    position: Qt.vector3d(0, 0, 1.5)
                                    eulerRotation.x: 90
                                    height: 3
                                    topDiameter: 0.5
                                    bottomDiameter: 0.4
                                }
                            ]
                        }
                    ]
                }
                objectLayer: moving
                motionType: Body.Dynamic

                Node {
                    id: subVisual1
                    eulerRotation: subCompound1.eulerRotation
                    Model {
                        source: "#Cube"
                        position: Qt.vector3d(0, 1.5, 0)
                        eulerRotation.z: 90
                        scale: Qt.vector3d(3 / 100, 0.5 / 100, 0.4 / 100)
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                    Model {
                        source: "#Cylinder"
                        position: Qt.vector3d(1.5, 0, 0)
                        eulerRotation.z: 90
                        scale: Qt.vector3d(0.4 / 100, 3 / 100, 0.4 / 100)
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                    Model {
                        geometry: TaperedCapsuleGeometry {
                            height: 3
                            topDiameter: 0.5
                            bottomDiameter: 0.4
                        }
                        position: Qt.vector3d(0, 0, 1.5)
                        eulerRotation.x: 90
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                }
                Node {
                    id: subVisual2
                    eulerRotation: subCompound2.eulerRotation
                    Model {
                        source: "#Cube"
                        position: Qt.vector3d(0, 1.5, 0)
                        eulerRotation.z: 90
                        scale: Qt.vector3d(3 / 100, 0.5 / 100, 0.4 / 100)
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                    Model {
                        source: "#Cylinder"
                        position: Qt.vector3d(1.5, 0, 0)
                        eulerRotation.z: 90
                        scale: Qt.vector3d(0.4 / 100, 3 / 100, 0.4 / 100)
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                    Model {
                        geometry: TaperedCapsuleGeometry {
                            height: 3
                            topDiameter: 0.5
                            bottomDiameter: 0.4
                        }
                        position: Qt.vector3d(0, 0, 1.5)
                        eulerRotation.x: 90
                        materials: PrincipledMaterial {
                            baseColor: colors[body.bodyID % colors.length]
                            metalness: 0.5
                            roughness: 0.1
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            for (let i = 0; i < 10; ++i) {
                const body = mutableBodyComponent.createObject(viewport.scene, {
                    position: Qt.vector3d(0, 10 + 5 * i, 0)
                })
                mutableBodies.push({
                    body: body,
                    compound: body.compound,
                    subShapes: [
                        { shape: body.sub1, visual: body.visual1 },
                        { shape: body.sub2, visual: body.visual2 }
                    ]
                })
            }
        }
    }

    SettingsView {}
}
