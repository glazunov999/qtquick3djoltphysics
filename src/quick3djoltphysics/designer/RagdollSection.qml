import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Ragdoll")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Collision Group Id")
                tooltip: qsTr("Collision Group Id")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 9999999
                    decimals: 0
                    backendValue: backendValues.collisionGroupId
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
