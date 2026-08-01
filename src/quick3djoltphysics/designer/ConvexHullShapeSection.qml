import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Convex Hull Shape")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Active Edge Cos Threshold Angle")
                tooltip: qsTr("Active Edge Cos Threshold Angle")
            }

            SecondColumnLayout {
                SpinBox {
                    minimumValue: -9999999
                    maximumValue: 9999999
                    decimals: 3
                    backendValue: backendValues.activeEdgeCosThresholdAngle
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}