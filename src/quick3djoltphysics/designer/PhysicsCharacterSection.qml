import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Physics Character")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Max Slope Angle")
                tooltip: qsTr("Max Slope Angle")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: 0
                    maximumValue: 90
                    decimals: 3
                    backendValue: backendValues.maxSlopeAngle
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Supporting Volume X")
                tooltip: qsTr("Supporting Volume X")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.supportingVolume_x
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Supporting Volume Y")
                tooltip: qsTr("Supporting Volume Y")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.supportingVolume_y
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Supporting Volume Z")
                tooltip: qsTr("Supporting Volume Z")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.supportingVolume_z
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Supporting Volume W")
                tooltip: qsTr("Supporting Volume W")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.supportingVolume_w
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}