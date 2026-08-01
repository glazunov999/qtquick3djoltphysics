import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Rotated Translated Shape")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Shape")
                tooltip: qsTr("Shape")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.Shape"
                    backendValue: backendValues.shape
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}