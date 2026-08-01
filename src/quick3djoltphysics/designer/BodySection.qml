import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Body")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Is Sensor")
                tooltip: qsTr("Whether the body is a sensor.")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.isSensor.valueToString
                    backendValue: backendValues.isSensor
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Collision Group")
                tooltip: qsTr("The collision group of the body.")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.CollisionGroup"
                    backendValue: backendValues.collisionGroup
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Motion Type")
                tooltip: qsTr("The motion type of the body.")
            }

            SecondColumnLayout {
                ComboBox {
                    scope: "Body"
                    model: ["Static", "Kinematic", "Dynamic"]
                    backendValue: backendValues.motionType
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Motion Quality")
                tooltip: qsTr("The motion quality of the body.")
            }

            SecondColumnLayout {
                ComboBox {
                    scope: "Body"
                    model: ["Discrete", "LinearCast"]
                    backendValue: backendValues.motionQuality
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Enhanced Internal Edge Removal")
                tooltip: qsTr("Enhanced Internal Edge Removal")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.enhancedInternalEdgeRemoval.valueToString
                    backendValue: backendValues.enhancedInternalEdgeRemoval
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
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
                text: qsTr("Override Mass Properties")
                tooltip: qsTr("Override Mass Properties")
            }

            SecondColumnLayout {
                ComboBox {
                    scope: "Body"
                    model: ["CalculateMassAndInertia", "CalculateInertia", "MassAndInertiaProvided"]
                    backendValue: backendValues.overrideMassProperties
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Mass")
                tooltip: qsTr("Mass")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 2
                    backendValue: backendValues.mass
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Offset Center Of Mass")
                tooltip: qsTr("Offset Center Of Mass")
            }
            SecondColumnLayout {
                SpinBox {
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.offsetCenterOfMass_x
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
                    decimals: 3
                    backendValue: backendValues.offsetCenterOfMass_y
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
                    decimals: 3
                    backendValue: backendValues.offsetCenterOfMass_z
                }

                Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

                ControlLabel {
                    text: "Z"
                    color: StudioTheme.Values.theme3DAxisZColor
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
                text: qsTr("Max Angular Velocity")
                tooltip: qsTr("Max Angular Velocity")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.maxAngularVelocity
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
                text: qsTr("Angular Damping")
                tooltip: qsTr("Angular Damping")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.angularDamping
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
                text: qsTr("Simulation Enabled")
                tooltip: qsTr("Simulation Enabled")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.simulationEnabled.valueToString
                    backendValue: backendValues.simulationEnabled
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}