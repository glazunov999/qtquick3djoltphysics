import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Soft Body")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Shared Settings")
                tooltip: qsTr("Shared Settings")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.SoftBodySharedSettings"
                    backendValue: backendValues.sharedSettings
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Object Layer")
                tooltip: qsTr("Object Layer")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.objectLayer
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Num Iterations")
                tooltip: qsTr("Num Iterations")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 1
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.numIterations
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Linear Damping")
                tooltip: qsTr("Linear Damping")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.linearDamping
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Max Linear Velocity")
                tooltip: qsTr("Max Linear Velocity")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.maxLinearVelocity
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Restitution")
                tooltip: qsTr("Restitution")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.restitution
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Friction")
                tooltip: qsTr("Friction")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.friction
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Pressure")
                tooltip: qsTr("Pressure")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.pressure
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Gravity Factor")
                tooltip: qsTr("Gravity Factor")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.gravityFactor
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Allow Sleeping")
                tooltip: qsTr("Allow Sleeping")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.allowSleeping.valueToString
                    backendValue: backendValues.allowSleeping
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Update Position")
                tooltip: qsTr("Update Position")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.updatePosition.valueToString
                    backendValue: backendValues.updatePosition
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Make Rotation Identity")
                tooltip: qsTr("Make Rotation Identity")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.makeRotationIdentity.valueToString
                    backendValue: backendValues.makeRotationIdentity
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}