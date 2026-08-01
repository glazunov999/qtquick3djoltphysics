import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Tapered Cylinder Geometry")
    width: parent.width

    SectionLayout {
        PropertyLabel {
            text: qsTr("Height")
            tooltip: qsTr("Sets the height of the tapered cylinder.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.height
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Top Diameter")
            tooltip: qsTr("Sets the diameter at the top of the cylinder.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.topDiameter
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Bottom Diameter")
            tooltip: qsTr("Sets the diameter at the bottom of the cylinder.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.bottomDiameter
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Longitudes")
            tooltip: qsTr("Sets the number of longitudinal subdivisions.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 3
                maximumValue: 999999
                decimals: 0
                backendValue: backendValues.longitudes
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Enable Normals")
            tooltip: qsTr("Sets whether vertex normals are generated.")
        }

        SecondColumnLayout {
            CheckBox {
                text: backendValues.enableNormals.valueToString
                backendValue: backendValues.enableNormals
                implicitWidth: StudioTheme.Values.twoControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Enable UV")
            tooltip: qsTr("Sets whether UV coordinates are generated.")
        }

        SecondColumnLayout {
            CheckBox {
                text: backendValues.enableUV.valueToString
                backendValue: backendValues.enableUV
                implicitWidth: StudioTheme.Values.twoControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }
    }
}
