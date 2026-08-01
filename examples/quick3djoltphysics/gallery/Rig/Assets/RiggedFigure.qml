import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics.Helpers

Node {
    id: root

    property alias skin: figureSkin

    readonly property real rotZ90: 90

    Skin {
        id: figureSkin
        joints: [
            lowerBody,
            midBody,
            upperBody,
            head,
            upperArmL,
            upperArmR,
            lowerArmL,
            lowerArmR,
            upperLegL,
            upperLegR,
            lowerLegL,
            lowerLegR
        ]
    }

    Node {
        id: lowerBody
        objectName: "LowerBody"
        position: Qt.vector3d(0, 1.15, 0)
        eulerRotation: Qt.vector3d(0, 0, rotZ90)
        Model {
            geometry: CapsuleGeometry { diameter: 0.20; height: 0.30 }
            materials: PrincipledMaterial { baseColor: "#c06060"; metalness: 0.35; roughness: 0.55 }
            castsShadows: true
            receivesShadows: true
        }

        Node {
            id: midBody
            objectName: "MidBody"
            position: Qt.vector3d(0.20, 0, 0)
            Model {
                geometry: CapsuleGeometry { diameter: 0.20; height: 0.30 }
                materials: PrincipledMaterial { baseColor: "#c07070"; metalness: 0.35; roughness: 0.55 }
                castsShadows: true
                receivesShadows: true
            }

            Node {
                id: upperBody
                objectName: "UpperBody"
                position: Qt.vector3d(0.20, 0, 0)
                Model {
                    geometry: CapsuleGeometry { diameter: 0.20; height: 0.30 }
                    materials: PrincipledMaterial { baseColor: "#c08080"; metalness: 0.35; roughness: 0.55 }
                    castsShadows: true
                    receivesShadows: true
                }

                Node {
                    id: head
                    objectName: "Head"
                    position: Qt.vector3d(0.275, 0, 0)
                    eulerRotation: Qt.vector3d(0, 0, -90)
                    Model {
                        geometry: CapsuleGeometry { diameter: 0.20; height: 0.15 }
                        materials: PrincipledMaterial { baseColor: "#d0a070"; metalness: 0.35; roughness: 0.55 }
                        castsShadows: true
                        receivesShadows: true
                    }
                }

                Node {
                    id: upperArmL
                    objectName: "UpperArmL"
                    position: Qt.vector3d(0, 0.425, 0)
                    Model {
                        geometry: CapsuleGeometry { diameter: 0.12; height: 0.30 }
                        materials: PrincipledMaterial { baseColor: "#7090c0"; metalness: 0.35; roughness: 0.55 }
                        castsShadows: true
                        receivesShadows: true
                    }

                    Node {
                        id: lowerArmL
                        objectName: "LowerArmL"
                        position: Qt.vector3d(0, 0.375, 0)
                        Model {
                            geometry: CapsuleGeometry { diameter: 0.10; height: 0.30 }
                            materials: PrincipledMaterial { baseColor: "#6080b0"; metalness: 0.35; roughness: 0.55 }
                            castsShadows: true
                            receivesShadows: true
                        }
                    }
                }

                Node {
                    id: upperArmR
                    objectName: "UpperArmR"
                    position: Qt.vector3d(0, -0.425, 0)
                    Model {
                        geometry: CapsuleGeometry { diameter: 0.12; height: 0.30 }
                        materials: PrincipledMaterial { baseColor: "#7090c0"; metalness: 0.35; roughness: 0.55 }
                        castsShadows: true
                        receivesShadows: true
                    }

                    Node {
                        id: lowerArmR
                        objectName: "LowerArmR"
                        position: Qt.vector3d(0, -0.375, 0)
                        Model {
                            geometry: CapsuleGeometry { diameter: 0.10; height: 0.30 }
                            materials: PrincipledMaterial { baseColor: "#6080b0"; metalness: 0.35; roughness: 0.55 }
                            castsShadows: true
                            receivesShadows: true
                        }
                    }
                }
            }
        }

        Node {
            id: upperLegL
            objectName: "UpperLegL"
            position: Qt.vector3d(-0.35, 0.15, 0)
            eulerRotation: Qt.vector3d(0, 0, -90)
            Model {
                geometry: CapsuleGeometry { diameter: 0.15; height: 0.40 }
                materials: PrincipledMaterial { baseColor: "#60a080"; metalness: 0.35; roughness: 0.55 }
                castsShadows: true
                receivesShadows: true
            }

            Node {
                id: lowerLegL
                objectName: "LowerLegL"
                position: Qt.vector3d(0, -0.5, 0)
                Model {
                    geometry: CapsuleGeometry { diameter: 0.12; height: 0.40 }
                    materials: PrincipledMaterial { baseColor: "#509070"; metalness: 0.35; roughness: 0.55 }
                    castsShadows: true
                    receivesShadows: true
                }
            }
        }

        Node {
            id: upperLegR
            objectName: "UpperLegR"
            position: Qt.vector3d(-0.35, -0.15, 0)
            eulerRotation: Qt.vector3d(0, 0, -90)
            Model {
                geometry: CapsuleGeometry { diameter: 0.15; height: 0.40 }
                materials: PrincipledMaterial { baseColor: "#60a080"; metalness: 0.35; roughness: 0.55 }
                castsShadows: true
                receivesShadows: true
            }

            Node {
                id: lowerLegR
                objectName: "LowerLegR"
                position: Qt.vector3d(0, -0.5, 0)
                Model {
                    geometry: CapsuleGeometry { diameter: 0.12; height: 0.40 }
                    materials: PrincipledMaterial { baseColor: "#509070"; metalness: 0.35; roughness: 0.55 }
                    castsShadows: true
                    receivesShadows: true
                }
            }
        }
    }
}
