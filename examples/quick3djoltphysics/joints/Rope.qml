import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers

Node {
    id: root

    property int nonMoving: 0
    property int moving: 1

    readonly property real linkHalfLength: 20
    readonly property real linkSpacing: 40

    GroupFilterTable {
        id: groupFilter
        numSubGroups: 7
        Component.onCompleted: {
            for (let i = 0; i < numSubGroups - 1; ++i)
                disableCollision(i, i + 1);
        }
    }

    Body {
        id: anchor
        position: Qt.vector3d(0, 0, 0)
        shape: EmptyShape {}
        objectLayer: nonMoving
        motionType: Body.Static
    }

    PointConstraint {
        body1: anchor
        body2: shape0
        point1: anchor.mapPositionToScene(Qt.vector3d(0, 0, 0))
        point2: shape0.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    PointConstraint {
        body1: shape0
        body2: shape1
        point1: shape0.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: shape1.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    PointConstraint {
        body1: shape1
        body2: shape2
        point1: shape1.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: shape2.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    PointConstraint {
        body1: shape2
        body2: shape3
        point1: shape2.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: shape3.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    PointConstraint {
        body1: shape3
        body2: shape4
        point1: shape3.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: shape4.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    PointConstraint {
        body1: shape4
        body2: shape5
        point1: shape4.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: shape5.mapPositionToScene(Qt.vector3d(0, linkHalfLength, 0))
    }

    FixedConstraint {
        body1: shape5
        body2: sphere
        point1: shape5.mapPositionToScene(Qt.vector3d(0, -linkHalfLength, 0))
        point2: sphere.mapPositionToScene(Qt.vector3d(0, 0, 0))
    }

    Body {
        id: shape0
        position: Qt.vector3d(0, -linkHalfLength, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 0
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: shape1
        position: Qt.vector3d(0, -linkHalfLength - linkSpacing, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 1
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: shape2
        position: Qt.vector3d(0, -linkHalfLength - 2 * linkSpacing, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 2
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: shape3
        position: Qt.vector3d(0, -linkHalfLength - 3 * linkSpacing, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 3
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: shape4
        position: Qt.vector3d(0, -linkHalfLength - 4 * linkSpacing, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 4
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: shape5
        position: Qt.vector3d(0, -linkHalfLength - 5 * linkSpacing, 0)
        shape: CapsuleShape {
            diameter: 10
            height: 40
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 5
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            eulerRotation: Qt.vector3d(0, 0, -90)
            geometry: CapsuleGeometry {
                diameter: 10
                height: 40
            }
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }

    Body {
        id: sphere
        position: Qt.vector3d(0, -linkHalfLength - 6 * linkSpacing, 0)
        scale: Qt.vector3d(0.5, 0.5, 0.5)
        shape: SphereShape {
            diameter: 100
        }
        collisionGroup: CollisionGroup {
            groupFilterTable: groupFilter
            groupId: 0
            subGroupId: 6
        }
        objectLayer: moving
        motionType: Body.Dynamic
        Model {
            source: "#Sphere"
            materials: PrincipledMaterial {
                baseColor: "blue"
            }
        }
    }
}
