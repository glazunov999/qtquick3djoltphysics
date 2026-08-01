import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Path Constraint")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Path")
                tooltip: qsTr("Path")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.PathConstraintPath"
                    backendValue: backendValues.path
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}