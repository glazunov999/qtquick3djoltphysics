import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import Example

Window {
    id: window

    width: 640
    height: 480
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Joints")

    readonly property int nonMoving: 0
    readonly property int moving: 1

    PhysicsSystem {
        scene: viewport.scene
        running: true
        gravity: Qt.vector3d(0, -981, 0)
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#d6dbdf"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            position: Qt.vector3d(0, 600, 700)
            eulerRotation: Qt.vector3d(-30, 0, 0)
            clipFar: 5000
            clipNear: 1
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            castsShadow: true
            brightness: 1
            shadowFactor: 50
            shadowMapQuality: Light.ShadowMapQualityHigh
        }

        Body {
            position: Qt.vector3d(0, -100, 0)
            eulerRotation: Qt.vector3d(-90, 0, 0)
            shape: PlaneShape {
                extent: 2000
            }
            objectLayer: nonMoving
            motionType: Body.Static
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(20, 20, 1)
                materials: PrincipledMaterial {
                    baseColor: "green"
                }
                castsShadows: false
                receivesShadows: true
            }
        }

        Rope {
            position: Qt.vector3d(0, 500, 0)
        }

        Prismatic {
            id: prismatic
            position: Qt.vector3d(-250, 100, 0)
            moving: window.moving
        }

        Revolute {
            id: revolute
            position: Qt.vector3d(200, 200, 100)
            moving: window.moving
        }

        FrameAnimation {
            running: true
            onTriggered: {
                prismatic.jointRotation = Qt.vector3d(
                    prismatic.jointRotation.x,
                    prismatic.jointRotation.y,
                    prismatic.jointRotation.z + 1)
                revolute.jointRotation = Qt.vector3d(
                    revolute.jointRotation.x + 0.95,
                    revolute.jointRotation.y,
                    revolute.jointRotation.z)
            }
        }
    }
}
