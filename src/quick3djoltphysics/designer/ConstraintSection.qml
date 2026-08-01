import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Constraint")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Body 1")
                tooltip: qsTr("The first body.")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.Body"
                    backendValue: backendValues.body1
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Body 2")
                tooltip: qsTr("The second body.")
            }

            SecondColumnLayout {
                ItemFilterComboBox {
                    typeFilter: "QtQuick3D.JoltPhysics.Body"
                    backendValue: backendValues.body2
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
            PropertyLabel {
                text: qsTr("Space")
                tooltip: qsTr("Constraint coordinate space.")
            }

            SecondColumnLayout {
                ComboBox {
                    scope: "PhysicsTwoBodyConstraint"
                    model: ["LocalToBodyCOM", "WorldSpace"]
                    backendValue: backendValues.space
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}