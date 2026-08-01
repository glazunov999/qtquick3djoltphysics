import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Physics System")
    width: parent.width

    SectionLayout {
        PropertyLabel {
            text: qsTr("Scene")
            tooltip: qsTr("The scene node to which the physics system is attached.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.Node"
                backendValue: backendValues.scene
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Viewport")
            tooltip: qsTr("The node to which the debug geometry of the physics system is added.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.Node"
                backendValue: backendValues.viewport
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Running")
            tooltip: qsTr("Whether the physics system is running.")
        }

        SecondColumnLayout {
            CheckBox {
                text: backendValues.running.valueToString
                backendValue: backendValues.running
                implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Force Debug Draw")
            tooltip: qsTr("Whether to force debug drawing of the physics system.")
        }

        SecondColumnLayout {
            CheckBox {
                text: backendValues.forceDebugDraw.valueToString
                backendValue: backendValues.forceDebugDraw
                implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Gravity")
            tooltip: qsTr("The gravity vector.")
        }

        SecondColumnLayout {
            SpinBox {
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
                minimumValue: -9999999
                maximumValue: 9999999
                decimals: 2
                backendValue: backendValues.gravity_x
            }

            Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

            ControlLabel {
                text: "X"
                color: StudioTheme.Values.theme3DAxisXColor
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
        }

        SecondColumnLayout {
            SpinBox {
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
                minimumValue: -9999999
                maximumValue: 9999999
                decimals: 2
                backendValue: backendValues.gravity_y
            }

            Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

            ControlLabel {
                text: "Y"
                color: StudioTheme.Values.theme3DAxisYColor
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
        }

        SecondColumnLayout {
            SpinBox {
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
                minimumValue: -9999999
                maximumValue: 9999999
                decimals: 2
                backendValue: backendValues.gravity_z
            }

            Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

            ControlLabel {
                text: "Z"
                color: StudioTheme.Values.theme3DAxisZColor
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Collision Steps")
            tooltip: qsTr("Number of collision detection steps per frame.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 1
                maximumValue: 9999999
                decimals: 0
                backendValue: backendValues.collisionSteps
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Min Timestep")
            tooltip: qsTr("Defines the minimum simulation timestep in milliseconds.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0.001
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.minimumTimestep
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Max Timestep")
            tooltip: qsTr("Defines the maximum simulation timestep in milliseconds.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0.001
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.maximumTimestep
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Num Bodies")
            tooltip: qsTr("Maximum number of bodies the physics system can hold.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 1
                maximumValue: 9999999
                decimals: 0
                backendValue: backendValues.numBodies
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Max Body Pairs")
            tooltip: qsTr("Maximum number of body pairs that can be tracked.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 1
                maximumValue: 9999999
                decimals: 0
                backendValue: backendValues.maxBodyPairs
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Max Contact Constraints")
            tooltip: qsTr("Maximum number of contact constraints.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 1
                maximumValue: 9999999
                decimals: 0
                backendValue: backendValues.maxContactConstraints
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Num Threads")
            tooltip: qsTr("Number of worker threads. -1 uses the ideal thread count.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: -1
                maximumValue: 9999999
                decimals: 0
                backendValue: backendValues.numThreads
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Settings")
            tooltip: qsTr("Physics settings object.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.PhysicsSettings"
                backendValue: backendValues.settings
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Object Layer Pair Filter")
            tooltip: qsTr("Filter that determines which object layers can collide.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.ObjectLayerPairFilter"
                backendValue: backendValues.objectLayerPairFilter
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Broad Phase Layer")
            tooltip: qsTr("Broad phase layer configuration.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.BroadPhaseLayer"
                backendValue: backendValues.broadPhaseLayer
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Object Vs Broad Phase Layer Filter")
            tooltip: qsTr("Filter between object layers and broad phase layers.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.ObjectVsBroadPhaseLayerFilter"
                backendValue: backendValues.objectVsBroadPhaseLayerFilter
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Contact Listener")
            tooltip: qsTr("Listener for contact events.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.ContactListener"
                backendValue: backendValues.contactListener
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Soft Body Contact Listener")
            tooltip: qsTr("Listener for soft body contact events.")
        }

        SecondColumnLayout {
            ItemFilterComboBox {
                typeFilter: "QtQuick3D.JoltPhysics.SoftBodyContactListener"
                backendValue: backendValues.softBodyContactListener
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }
    }
}
