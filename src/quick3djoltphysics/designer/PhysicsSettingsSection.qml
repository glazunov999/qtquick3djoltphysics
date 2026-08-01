import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Physics Settings")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Max In Flight Body Pairs")
                tooltip: qsTr("Max In Flight Body Pairs")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 1
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.maxInFlightBodyPairs
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Num Velocity Steps")
                tooltip: qsTr("Num Velocity Steps")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 1
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.numVelocitySteps
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Num Position Steps")
                tooltip: qsTr("Num Position Steps")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 1
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.numPositionSteps
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Baumgarte")
                tooltip: qsTr("Baumgarte")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.baumgarte
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Time Before Sleep")
                tooltip: qsTr("Time Before Sleep")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.timeBeforeSleep
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
                text: qsTr("Deterministic Simulation")
                tooltip: qsTr("Deterministic Simulation")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.deterministicSimulation.valueToString
                    backendValue: backendValues.deterministicSimulation
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Constraint Warm Start")
                tooltip: qsTr("Constraint Warm Start")
            }

            SecondColumnLayout {
                CheckBox {
                    text: backendValues.constraintWarmStart.valueToString
                    backendValue: backendValues.constraintWarmStart
                    implicitWidth: StudioTheme.Values.twoControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}