import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers

Ragdoll {
    id: root

    motionType: Body.Dynamic

    readonly property real rotZ90: 90

    parts: [
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

    RagdollPart {
        id: lowerBody
        objectName: "LowerBody"
        position: Qt.vector3d(0, 1.15, 0)
        eulerRotation: Qt.vector3d(0, 0, rotZ90)
        shape: CapsuleShape { diameter: 0.20; height: 0.30 }

        RagdollPart {
            id: midBody
            objectName: "MidBody"
            position: Qt.vector3d(0.20, 0, 0)
            shape: CapsuleShape { diameter: 0.20; height: 0.30 }
            constraintToParent: SwingTwistConstraintSettings {
                position1: Qt.vector3d(0.10, 0, 0)
                position2: Qt.vector3d(0.10, 0, 0)
                twistAxis1: Qt.vector3d(1, 0, 0)
                twistAxis2: Qt.vector3d(1, 0, 0)
                planeAxis1: Qt.vector3d(0, 0, 1)
                planeAxis2: Qt.vector3d(0, 0, 1)
                twistMinAngle: -5
                twistMaxAngle: 5
                normalHalfConeAngle: 10
                planeHalfConeAngle: 10
            }

            RagdollPart {
                id: upperBody
                objectName: "UpperBody"
                position: Qt.vector3d(0.20, 0, 0)
                shape: CapsuleShape { diameter: 0.20; height: 0.30 }
                constraintToParent: SwingTwistConstraintSettings {
                    position1: Qt.vector3d(0.10, 0, 0)
                    position2: Qt.vector3d(0.10, 0, 0)
                    twistAxis1: Qt.vector3d(1, 0, 0)
                    twistAxis2: Qt.vector3d(1, 0, 0)
                    planeAxis1: Qt.vector3d(0, 0, 1)
                    planeAxis2: Qt.vector3d(0, 0, 1)
                    twistMinAngle: -5
                    twistMaxAngle: 5
                    normalHalfConeAngle: 10
                    planeHalfConeAngle: 10
                }

                RagdollPart {
                    id: head
                    objectName: "Head"
                    position: Qt.vector3d(0.275, 0, 0)
                    eulerRotation: Qt.vector3d(0, 0, -90)
                    shape: CapsuleShape { diameter: 0.20; height: 0.15 }
                    constraintToParent: SwingTwistConstraintSettings {
                        position1: Qt.vector3d(0.10, 0, 0)
                        position2: Qt.vector3d(0.10, 0, 0)
                        twistAxis1: Qt.vector3d(1, 0, 0)
                        twistAxis2: Qt.vector3d(1, 0, 0)
                        planeAxis1: Qt.vector3d(0, 0, 1)
                        planeAxis2: Qt.vector3d(0, 0, 1)
                        twistMinAngle: -90
                        twistMaxAngle: 90
                        normalHalfConeAngle: 45
                        planeHalfConeAngle: 45
                    }
                }

                RagdollPart {
                    id: upperArmL
                    objectName: "UpperArmL"
                    position: Qt.vector3d(0, 0.425, 0)
                    shape: CapsuleShape { diameter: 0.12; height: 0.30 }
                    constraintToParent: SwingTwistConstraintSettings {
                        position1: Qt.vector3d(0, 0.225, 0)
                        position2: Qt.vector3d(0, 0.225, 0)
                        twistAxis1: Qt.vector3d(0, 1, 0)
                        twistAxis2: Qt.vector3d(0, 1, 0)
                        planeAxis1: Qt.vector3d(0, 0, 1)
                        planeAxis2: Qt.vector3d(0, 0, 1)
                        twistMinAngle: -45
                        twistMaxAngle: 45
                        normalHalfConeAngle: 90
                        planeHalfConeAngle: 45
                    }

                    RagdollPart {
                        id: lowerArmL
                        objectName: "LowerArmL"
                        position: Qt.vector3d(0, 0.375, 0)
                        shape: CapsuleShape { diameter: 0.10; height: 0.30 }
                        constraintToParent: SwingTwistConstraintSettings {
                            position1: Qt.vector3d(0, 0.225, 0)
                            position2: Qt.vector3d(0, 0.225, 0)
                            twistAxis1: Qt.vector3d(0, 1, 0)
                            twistAxis2: Qt.vector3d(0, 1, 0)
                            planeAxis1: Qt.vector3d(0, 0, 1)
                            planeAxis2: Qt.vector3d(0, 0, 1)
                            twistMinAngle: -45
                            twistMaxAngle: 45
                            normalHalfConeAngle: 0
                            planeHalfConeAngle: 90
                        }
                    }
                }

                RagdollPart {
                    id: upperArmR
                    objectName: "UpperArmR"
                    position: Qt.vector3d(0, -0.425, 0)
                    shape: CapsuleShape { diameter: 0.12; height: 0.30 }
                    constraintToParent: SwingTwistConstraintSettings {
                        position1: Qt.vector3d(0, -0.225, 0)
                        position2: Qt.vector3d(0, -0.225, 0)
                        twistAxis1: Qt.vector3d(0, -1, 0)
                        twistAxis2: Qt.vector3d(0, -1, 0)
                        planeAxis1: Qt.vector3d(0, 0, 1)
                        planeAxis2: Qt.vector3d(0, 0, 1)
                        twistMinAngle: -45
                        twistMaxAngle: 45
                        normalHalfConeAngle: 90
                        planeHalfConeAngle: 45
                    }

                    RagdollPart {
                        id: lowerArmR
                        objectName: "LowerArmR"
                        position: Qt.vector3d(0, -0.375, 0)
                        shape: CapsuleShape { diameter: 0.10; height: 0.30 }
                        constraintToParent: SwingTwistConstraintSettings {
                            position1: Qt.vector3d(0, -0.225, 0)
                            position2: Qt.vector3d(0, -0.225, 0)
                            twistAxis1: Qt.vector3d(0, -1, 0)
                            twistAxis2: Qt.vector3d(0, -1, 0)
                            planeAxis1: Qt.vector3d(0, 0, 1)
                            planeAxis2: Qt.vector3d(0, 0, 1)
                            twistMinAngle: -45
                            twistMaxAngle: 45
                            normalHalfConeAngle: 0
                            planeHalfConeAngle: 90
                        }
                    }
                }
            }
        }

        RagdollPart {
            id: upperLegL
            objectName: "UpperLegL"
            position: Qt.vector3d(-0.35, 0.15, 0)
            eulerRotation: Qt.vector3d(0, 0, -90)
            shape: CapsuleShape { diameter: 0.15; height: 0.40 }
            constraintToParent: SwingTwistConstraintSettings {
                position1: Qt.vector3d(-0.10, 0.15, 0)
                position2: Qt.vector3d(-0.10, 0.15, 0)
                twistAxis1: Qt.vector3d(-1, 0, 0)
                twistAxis2: Qt.vector3d(-1, 0, 0)
                planeAxis1: Qt.vector3d(0, 0, 1)
                planeAxis2: Qt.vector3d(0, 0, 1)
                twistMinAngle: -45
                twistMaxAngle: 45
                normalHalfConeAngle: 45
                planeHalfConeAngle: 45
            }

            RagdollPart {
                id: lowerLegL
                objectName: "LowerLegL"
                position: Qt.vector3d(0, -0.5, 0)
                shape: CapsuleShape { diameter: 0.12; height: 0.40 }
                constraintToParent: SwingTwistConstraintSettings {
                    position1: Qt.vector3d(0, -0.25, 0)
                    position2: Qt.vector3d(0, -0.25, 0)
                    twistAxis1: Qt.vector3d(0, -1, 0)
                    twistAxis2: Qt.vector3d(0, -1, 0)
                    planeAxis1: Qt.vector3d(0, 0, 1)
                    planeAxis2: Qt.vector3d(0, 0, 1)
                    twistMinAngle: -45
                    twistMaxAngle: 45
                    normalHalfConeAngle: 0
                    planeHalfConeAngle: 60
                }
            }
        }

        RagdollPart {
            id: upperLegR
            objectName: "UpperLegR"
            position: Qt.vector3d(-0.35, -0.15, 0)
            eulerRotation: Qt.vector3d(0, 0, -90)
            shape: CapsuleShape { diameter: 0.15; height: 0.40 }
            constraintToParent: SwingTwistConstraintSettings {
                position1: Qt.vector3d(-0.10, -0.15, 0)
                position2: Qt.vector3d(-0.10, -0.15, 0)
                twistAxis1: Qt.vector3d(-1, 0, 0)
                twistAxis2: Qt.vector3d(-1, 0, 0)
                planeAxis1: Qt.vector3d(0, 0, 1)
                planeAxis2: Qt.vector3d(0, 0, 1)
                twistMinAngle: -45
                twistMaxAngle: 45
                normalHalfConeAngle: 45
                planeHalfConeAngle: 45
            }

            RagdollPart {
                id: lowerLegR
                objectName: "LowerLegR"
                position: Qt.vector3d(0, -0.5, 0)
                shape: CapsuleShape { diameter: 0.12; height: 0.40 }
                constraintToParent: SwingTwistConstraintSettings {
                    position1: Qt.vector3d(0, -0.25, 0)
                    position2: Qt.vector3d(0, -0.25, 0)
                    twistAxis1: Qt.vector3d(0, -1, 0)
                    twistAxis2: Qt.vector3d(0, -1, 0)
                    planeAxis1: Qt.vector3d(0, 0, 1)
                    planeAxis2: Qt.vector3d(0, 0, 1)
                    twistMinAngle: -45
                    twistMaxAngle: 45
                    normalHalfConeAngle: 0
                    planeHalfConeAngle: 60
                }
            }
        }
    }
}
