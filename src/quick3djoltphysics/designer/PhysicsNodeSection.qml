import QtQuick 2.15
import QtQuick.Layouts 1.15
import HelperWidgets 2.0
import StudioTheme 1.0 as StudioTheme

Section {
    caption: qsTr("Physics Node")
    width: parent.width

    SectionLayout {
            PropertyLabel {
                text: qsTr("Activation")
                tooltip: qsTr("Activation mode when the physics object is created.")
            }

            SecondColumnLayout {
                ComboBox {
                    scope: "PhysicsNode"
                    model: ["Activate", "DontActivate"]
                    backendValue: backendValues.activation
                    implicitWidth: StudioTheme.Values.singleControlColumnWidth
                                    + StudioTheme.Values.actionIndicatorWidth
                }

                ExpandingSpacer {}
            }
        }
    
}