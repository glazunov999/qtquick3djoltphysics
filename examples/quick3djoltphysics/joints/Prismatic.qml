import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Node {
    id: root

    property int moving: 1
    property vector3d jointRotation: Qt.vector3d(0, 0, 0)

    readonly property vector3d boxExtents: Qt.vector3d(200, 50, 50)
    readonly property vector3d boxScale: Qt.vector3d(2, 0.5, 0.5)
    readonly property real halfX: boxExtents.x / 2

    GroupFilterTable {
        id: pairFilter
        numSubGroups: 2
        Component.onCompleted: disableCollision(0, 1)
    }

    Body {
        id: prismaticBoxA
        kinematicPosition: Qt.vector3d(0, 200, 0)
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
            groupId: 1
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
        id: prismaticBoxB
        position: Qt.vector3d(halfX, 200, 0)
        shape: BoxShape {
            extents: Qt.vector3d(boxExtents.x, boxExtents.y * 0.8, boxExtents.z * 0.8)
        }
        objectLayer: moving
        motionType: Body.Dynamic
        collisionGroup: CollisionGroup {
            groupFilterTable: pairFilter
            groupId: 1
            subGroupId: 1
        }
        Model {
            source: "#Cube"
            scale: Qt.vector3d(boxScale.x, boxScale.y * 0.8, boxScale.z * 0.8)
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    SliderConstraint {
        body1: prismaticBoxA
        body2: prismaticBoxB
        space: SliderConstraint.LocalToBodyCOM
        autoDetectPoint: false
        point1: Qt.vector3d(halfX, 0, 0)
        point2: Qt.vector3d(-halfX, 0, 0)
        sliderAxis1: Qt.vector3d(1, 0, 0)
        sliderAxis2: Qt.vector3d(1, 0, 0)
        normalAxis1: Qt.vector3d(0, 1, 0)
        normalAxis2: Qt.vector3d(0, 1, 0)
        limitsMin: -200
        limitsMax: 0
    }
}
