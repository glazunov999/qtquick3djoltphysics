import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Node {
    id: root

    property int moving: 1

    property vector3d jointRotation: Qt.vector3d(0, 90, 0)

    readonly property vector3d boxExtents: Qt.vector3d(200, 50, 50)
    readonly property vector3d boxScale: Qt.vector3d(2, 0.5, 0.5)
    readonly property real attachX: boxExtents.x / 2

    GroupFilterTable {
        id: pairFilter
        numSubGroups: 2
        Component.onCompleted: disableCollision(0, 1)
    }

    Body {
        id: revoluteBoxA
        kinematicPosition: Qt.vector3d(0, 0, 0)
        kinematicEulerRotation: jointRotation
        position: kinematicPosition
        eulerRotation: jointRotation
        shape: BoxShape {
            extents: boxExtents
        }
        objectLayer: moving
        motionType: Body.Kinematic
        collisionGroup: CollisionGroup {
            groupFilterTable: pairFilter
            groupId: 2
            subGroupId: 0
        }
        Model {
            source: "#Cube"
            scale: boxScale
            materials: PrincipledMaterial {
                baseColor: "yellow"
            }
        }
    }

    Body {
        id: revoluteBoxB
        position: Qt.vector3d(0, 100, -100)
        eulerRotation: Qt.vector3d(0, 90, -90)
        shape: BoxShape {
            extents: boxExtents
        }
        objectLayer: moving
        motionType: Body.Dynamic
        collisionGroup: CollisionGroup {
            groupFilterTable: pairFilter
            groupId: 2
            subGroupId: 1
        }
        Model {
            source: "#Cube"
            scale: boxScale
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    HingeConstraint {
        body1: revoluteBoxA
        body2: revoluteBoxB
        space: HingeConstraint.LocalToBodyCOM
        point1: Qt.vector3d(attachX, 0, 0)
        point2: Qt.vector3d(attachX, 0, 0)
        hingeAxis1: Qt.vector3d(1, 0, 0)
        normalAxis1: Qt.vector3d(0, 1, 0)
        hingeAxis2: Qt.vector3d(0, 1, 0)
        normalAxis2: Qt.vector3d(-1, 0, 0)
        limitsMin: -45
        limitsMax: 45
    }
}
