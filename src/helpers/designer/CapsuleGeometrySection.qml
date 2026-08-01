import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Capsule Geometry")
    width: parent.width

    SectionLayout {
        PropertyLabel {
            text: qsTr("Height")
            tooltip: qsTr("Sets the height of the capsule.")
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
            text: qsTr("Diameter")
            tooltip: qsTr("Sets the diameter of the capsule.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 9999999
                decimals: 3
                backendValue: backendValues.diameter
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
            text: qsTr("Latitudes")
            tooltip: qsTr("Sets the number of latitudinal subdivisions.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 1
                maximumValue: 999999
                decimals: 0
                backendValue: backendValues.latitudes
                implicitWidth: StudioTheme.Values.singleControlColumnWidth
                               + StudioTheme.Values.actionIndicatorWidth
            }

            ExpandingSpacer {}
        }

        PropertyLabel {
            text: qsTr("Rings")
            tooltip: qsTr("Sets the number of rings in the cylindrical section.")
        }

        SecondColumnLayout {
            SpinBox {
                minimumValue: 0
                maximumValue: 999999
                decimals: 0
                backendValue: backendValues.rings
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
