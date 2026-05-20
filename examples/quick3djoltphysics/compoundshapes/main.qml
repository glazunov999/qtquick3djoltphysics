import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import Example

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Compound Shapes")

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: true
        gravity: Qt.vector3d(0, -981, 0)
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        collisionSteps: 24
        settings: PhysicsSettings {
            numPositionSteps: 20
            numVelocitySteps: 100
            baumgarte: 1
        }
    }

    View3D {
        id: viewport
        property real ringY: 900
        property real ringDistance: 165
        anchors.fill: parent

        environment: SceneEnvironment {
            antialiasingMode: SceneEnvironment.MSAA
            backgroundMode: SceneEnvironment.Color
            clearColor: "lightblue"
        }

        focus: true

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 900, 1500)
            eulerRotation: Qt.vector3d(-10, 0, 0)
            clipFar: 15500
            clipNear: 1
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            castsShadow: true
            brightness: 1.5
            shadowFactor: 15
            shadowFilter: 10
            shadowMapFar: 100
            shadowBias: -0.01
            shadowMapQuality: Light.ShadowMapQualityVeryHigh
        }

        Body {
            position: Qt.vector3d(0, -100, 0)
            eulerRotation: Qt.vector3d(-90, 0, 0)
            shape: PlaneShape {
                extent: 50000
            }
            objectLayer: nonMoving
            motionType: Body.Static
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(500, 500, 1)
                materials: DefaultMaterial {
                    diffuseColor: "green"
                }
                castsShadows: false
                receivesShadows: true
            }
        }

        MeshLink {
            id: leftLink
            motionType: Body.Kinematic
            property vector3d startPos: Qt.vector3d(-6 * viewport.ringDistance,
                                                    viewport.ringY,
                                                    0)
            property vector3d startRot: Qt.vector3d(90, 0, 0)
            position: startPos
            kinematicPosition: position
            kinematicEulerRotation: startRot
            eulerRotation: startRot
            color: "red"
        }

        CapsuleLink {
            position: Qt.vector3d(-5 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        MeshLink {
            position: Qt.vector3d(-4 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        MeshLink {
            position: Qt.vector3d(-3 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(0, 90, 0)
        }

        MeshLink {
            position: Qt.vector3d(-2 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        MeshLink {
            position: Qt.vector3d(-1 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(0, 90, 0)
        }

        CapsuleLink {
            position: Qt.vector3d(0, viewport.ringY, 0)
        }

        MeshLink {
            position: Qt.vector3d(1 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(0, 90, 0)
        }

        MeshLink {
            position: Qt.vector3d(2 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        MeshLink {
            position: Qt.vector3d(3 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(0, 90, 0)
        }

        MeshLink {
            position: Qt.vector3d(4 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        CapsuleLink {
            position: Qt.vector3d(5 * viewport.ringDistance, viewport.ringY, 0)
            eulerRotation: Qt.vector3d(90, 0, 0)
        }

        MeshLink {
            id: rightLink
            motionType: Body.Kinematic
            property vector3d startPos: Qt.vector3d(6 * viewport.ringDistance,
                                                    viewport.ringY,
                                                    0)
            property vector3d startRot: Qt.vector3d(90, 0, 0)
            position: startPos
            kinematicPosition: position
            kinematicEulerRotation: startRot
            eulerRotation: startRot
            color: "red"
        }

        Connections {
            target: physicsSystem
            property real totalAnimationTime: 12000
            function onFrameDone(timeStep) {
                let progressStep = timeStep * 1000 / totalAnimationTime
                animationController.progress += progressStep
                if (animationController.progress >= 1) {
                    animationController.completeToEnd()
                    animationController.reload()
                    animationController.progress = 0
                }
            }
        }

        AnimationController {
            id: animationController
            animation: SequentialAnimation {
                NumberAnimation {
                    target: leftLink
                    property: "kinematicPosition.x"
                    to: 3 * viewport.ringDistance
                    from: -6 * viewport.ringDistance
                    easing.type: Easing.InOutCubic
                    duration: 1000
                }
                NumberAnimation {
                    target: leftLink
                    property: "kinematicPosition.x"
                    from: 3 * viewport.ringDistance
                    to: -6 * viewport.ringDistance
                    easing.type: Easing.InOutCubic
                    duration: 1000
                }
                NumberAnimation {
                    target: rightLink
                    property: "kinematicPosition.x"
                    to: -3 * viewport.ringDistance
                    from: 6 * viewport.ringDistance
                    easing.type: Easing.InOutCubic
                    duration: 1000
                }
                NumberAnimation {
                    target: rightLink
                    property: "kinematicPosition.x"
                    from: -3 * viewport.ringDistance
                    to: 6 * viewport.ringDistance
                    easing.type: Easing.InOutCubic
                    duration: 1000
                }
            }
        }
    }
}
