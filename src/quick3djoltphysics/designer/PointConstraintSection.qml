import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Point Constraint")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Point 1")
                tooltip: qsTr("Point 1")
            }
            SecondColumnLayout {
                SpinBox {
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.point1_x
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
                    backendValue: backendValues.point1_y
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
                    backendValue: backendValues.point1_z
                }

                Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

                ControlLabel {
                    text: "Z"
                    color: StudioTheme.Values.theme3DAxisZColor
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Point 2")
                tooltip: qsTr("Point 2")
            }
            SecondColumnLayout {
                SpinBox {
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.point2_x
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
                    backendValue: backendValues.point2_y
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
                    backendValue: backendValues.point2_z
                }

                Spacer { implicitWidth: StudioTheme.Values.controlLabelGap }

                ControlLabel {
                    text: "Z"
                    color: StudioTheme.Values.theme3DAxisZColor
                }

                ExpandingSpacer {}
            }
        }
    
}