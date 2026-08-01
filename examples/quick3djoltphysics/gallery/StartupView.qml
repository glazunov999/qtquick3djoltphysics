pragma ComponentBehavior: Bound

import QtQuick
import QtQuick3D
import QtQuick3D.Particles3D
import QtQuick.Controls
import QtQuick3D.JoltPhysics
import Example

Item {
    id: mainView

    required property Loader loader

    readonly property real listItemWidth: 180
    readonly property real listItemHeight: 40
    readonly property real listColumnSpacing: 20

    anchors.fill: parent

    function maxListModelCount(models) {
        let max = 0
        for (let i = 0; i < models.length; ++i)
            max = Math.max(max, models[i].count)
        return max
    }

    ListModel {
        id: generalModel
        ListElement {
            name: "Simple"
            file: "General/SimpleTest.qml"
        }
        ListElement {
            name: "Stack"
            file: "General/StackTest.qml"
        }
        ListElement {
            name: "Wall"
            file: "General/WallTest.qml"
        }
        ListElement {
            name: "Pyramid"
            file: "General/PyramidTest.qml"
        }
        ListElement {
            name: "2D Funnel"
            file: "General/TwoDFunnelTest.qml"
        }
        ListElement {
            name: "Friction"
            file: "General/FrictionTest.qml"
        }
        ListElement {
            name: "Gravity Factor"
            file: "General/GravityFactorTest.qml"
        }
        ListElement {
            name: "Restitution"
            file: "General/RestitutionTest.qml"
        }
        ListElement {
            name: "Damping"
            file: "General/DampingTest.qml"
        }
        ListElement {
            name: "High Speed"
            file: "General/HighSpeedTest.qml"
        }
        ListElement {
            name: "Kinematic"
            file: "General/KinematicTest.qml"
        }
        ListElement {
            name: "Contact Manifold"
            file: "General/ContactManifoldTest.qml"
        }
        ListElement {
            name: "Change Motion Quality"
            file: "General/ChangeMotionQualityTest.qml"
        }
        ListElement {
            name: "Change Object Layer"
            file: "General/ChangeObjectLayerTest.qml"
        }
        ListElement {
            name: "Change Shape"
            file: "General/ChangeShapeTest.qml"
        }
        ListElement {
            name: "Contact Listener"
            file: "General/ContactListenerTest.qml"
        }
        ListElement {
            name: "Sensor"
            file: "General/SensorTest.qml"
        }
        ListElement {
            name: "Center Of Mass"
            file: "General/CenterOfMassTest.qml"
        }
    }

    ListModel {
        id: shapesModel
        ListElement {
            name: "Shpere Shape"
            file: "Shapes/SphereShapeTest.qml"
        }
        ListElement {
            name: "Box Shape"
            file: "Shapes/BoxShapeTest.qml"
        }
        ListElement {
            name: "Capsule Shape"
            file: "Shapes/CapsuleShapeTest.qml"
        }
        ListElement {
            name: "Tapered Capsule Shape"
            file: "Shapes/TaperedCapsuleShapeTest.qml"
        }
        ListElement {
            name: "Tapered Cylinder Shape"
            file: "Shapes/TaperedCylinderShapeTest.qml"
        }
        ListElement {
            name: "Cylinder Shape"
            file: "Shapes/CylinderShapeTest.qml"
        }
        ListElement {
            name: "Convex Hull Shape"
            file: "Shapes/ConvexHullShapeTest.qml"
        }
        ListElement {
            name: "Mesh Shape"
            file: "Shapes/MeshShapeTest.qml"
        }
        ListElement {
            name: "HeightField Shape"
            file: "Shapes/HeightFieldShapeTest.qml"
        }
        ListElement {
            name: "Static Compound Shape"
            file: "Shapes/StaticCompoundShapeTest.qml"
        }
        ListElement {
            name: "Mutable Compound Shape"
            file: "Shapes/MutableCompoundShapeTest.qml"
        }
        ListElement {
            name: "Offset Center Of Mass Shape"
            file: "Shapes/OffsetCenterOfMassShapeTest.qml"
        }
        ListElement {
            name: "Plane Shape"
            file: "Shapes/PlaneShapeTest.qml"
        }
    }

    ListModel {
        id: scaledShapesModel
        ListElement {
            name: "Scaled Sphere Shape"
            file: "ScaledShapes/ScaledSphereShapeTest.qml"
        }
        ListElement {
            name: "Scaled Box Shape"
            file: "ScaledShapes/ScaledBoxShapeTest.qml"
        }
        ListElement {
            name: "Scaled Capsule Shape"
            file: "ScaledShapes/ScaledCapsuleShapeTest.qml"
        }
        ListElement {
            name: "Scaled Cylinder Shape"
            file: "ScaledShapes/ScaledCylinderShapeTest.qml"
        }
        ListElement {
            name: "Scaled Tapered Capsule Shape"
            file: "ScaledShapes/ScaledTaperedCapsuleShapeTest.qml"
        }
        ListElement {
            name: "Scaled Tapered Cylinder Shape"
            file: "ScaledShapes/ScaledTaperedCylinderShapeTest.qml"
        }
        ListElement {
            name: "Scaled Convex Hull Shape"
            file: "ScaledShapes/ScaledConvexHullShapeTest.qml"
        }
        ListElement {
            name: "Scaled Static Compound Shape"
            file: "ScaledShapes/ScaledStaticCompoundShapeTest.qml"
        }
        ListElement {
            name: "Scaled Mutable Compound Shape"
            file: "ScaledShapes/ScaledMutableCompoundShapeTest.qml"
        }
        ListElement {
            name: "Scaled Offset Center Of Mass Shape"
            file: "ScaledShapes/ScaledOffsetCenterOfMassShapeTest.qml"
        }
        ListElement {
            name: "Scaled HeightField Shape"
            file: "ScaledShapes/ScaledHeightFieldShapeTest.qml"
        }
        ListElement {
            name: "Scaled Mesh Shape"
            file: "ScaledShapes/ScaledMeshShapeTest.qml"
        }
        ListElement {
            name: "Scaled Plane Shape"
            file: "ScaledShapes/ScaledPlaneShapeTest.qml"
        }
        ListElement {
            name: "Dynamic Scaled Shape"
            file: "ScaledShapes/DynamicScaledShapeTest.qml"
        }
    }

    ListModel {
        id: constraintsModel
        ListElement {
            name: "Point Constraint"
            file: "Constraints/PointConstraintTest.qml"
        }
        ListElement {
            name: "Cone Constraint"
            file: "Constraints/ConeConstraintTest.qml"
        }
        ListElement {
            name: "Distance Constraint"
            file: "Constraints/DistanceConstraintTest.qml"
        }
        ListElement {
            name: "Hinge Constraint"
            file: "Constraints/HingeConstraintTest.qml"
        }
        ListElement {
            name: "Slider Constraint"
            file: "Constraints/SliderConstraintTest.qml"
        }
        ListElement {
            name: "Swing Twist Constraint"
            file: "Constraints/SwingTwistConstraintTest.qml"
        }
        ListElement {
            name: "Fixed Constraint"
            file: "Constraints/FixedConstraintTest.qml"
        }
        ListElement {
            name: "Pulley Constraint"
            file: "Constraints/PulleyConstraintTest.qml"
        }
        ListElement {
            name: "Gear Constraint"
            file: "Constraints/GearConstraintTest.qml"
        }
        ListElement {
            name: "Path Constraint"
            file: "Constraints/PathConstraintTest.qml"
        }
        ListElement {
            name: "Rack And Pinion Constraint"
            file: "Constraints/RackAndPinionConstraintTest.qml"
        }
    }

    ListModel {
        id: characterModel
        ListElement {
            name: "Character"
            file: "Character/CharacterTest.qml"
        }
        ListElement {
            name: "Character Virtual"
            file: "Character/CharacterVirtualTest.qml"
        }
    }

    ListModel {
        id: rigModel
        ListElement {
            name: "Create Rig"
            file: "Rig/CreateRigTest.qml"
        }
        ListElement {
            name: "Skeleton Mapper"
            file: "Rig/SkeletonMapperTest.qml"
        }
    }

    ListModel {
        id: softBodyModel
        ListElement {
            name: "Gravity Factor"
            file: "SoftBody/SoftBodyGravityTest.qml"
        }
        ListElement {
            name: "Kinematic"
            file: "SoftBody/SoftBodyKinematicTest.qml"
        }
        ListElement {
            name: "Force"
            file: "SoftBody/SoftBodyForceTest.qml"
        }
        ListElement {
            name: "Restitution"
            file: "SoftBody/SoftBodyRestitutionTest.qml"
        }
        ListElement {
            name: "Contact Listener"
            file: "SoftBody/SoftBodyContactListenerTest.qml"
        }
        ListElement {
            name: "Pressure"
            file: "SoftBody/SoftBodyPressureTest.qml"
        }
    }

    readonly property var listModels: [
        generalModel,
        shapesModel,
        scaledShapesModel,
        constraintsModel,
        characterModel,
        rigModel,
        softBodyModel
    ]
    readonly property int maxListItemCount: maxListModelCount(listModels)
    readonly property real listsWidth: listModels.length * listItemWidth
                                     + (listModels.length - 1) * listColumnSpacing
    readonly property real listsHeight: maxListItemCount * listItemHeight

    Component {
        id: listComponent
        Button {
            id: button
            required property string name
            required property string file

            width: mainView.listItemWidth
            height: mainView.listItemHeight
            background: Rectangle {
                id: buttonBackground
                border.width: 0.5
                border.color: "#d0808080"
                color: "#d0404040"
                opacity: button.hovered ? 1.0 : 0.5
            }
            contentItem: Text {
                anchors.centerIn: parent
                color: "#f0f0f0"
                font.pointSize: AppSettings.fontSizeSmall
                text: button.name
            }

            onClicked: {
                mainView.loader.source = button.file
            }
        }
    }

    Text {
        id: topLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        text: qsTr("Qt Quick 3D Jolt Physics - Gallery")
        color: "#f0f0f0"
        font.pointSize: AppSettings.fontSizeLarge
    }

    Flickable {
        id: listsFlickable
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topLabel.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        contentWidth: Math.max(width, listsWidth)
        contentHeight: listsHeight + 20
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.horizontal: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }

        Row {
            id: listsRow
            x: (listsFlickable.contentWidth - width) / 2
            spacing: listColumnSpacing

            Repeater {
                model: listModels
                delegate: ListView {
                    required property var modelData

                    width: mainView.listItemWidth
                    height: count * mainView.listItemHeight
                    interactive: false
                    model: modelData
                    delegate: listComponent
                }
            }
        }
    }
}
