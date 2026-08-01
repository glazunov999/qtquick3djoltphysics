import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.JoltPhysics
import Example

import ".."

Item {
    id: mainWindow

    anchors.fill: parent

    readonly property int nonMoving: 0
    readonly property int moving: 1

    readonly property real worldScale: 0.2

    function tryInitializeMapper() {
        if (mapper.initialized)
            return
        if (sourceRagdoll.bodyCount() === 0)
            return

        sourceRagdoll.getPose(neutralPose)
        if (!mapper.initialize()) {
            console.warn("SkeletonMapperTest: mapper initialization failed")
            return
        }
    }

    function syncTargetFigure() {
        if (!mapper.initialized)
            return

        sourceRagdoll.getPose(ragdollPose)
        mapper.map(ragdollPose)
    }

    SkeletonPose {
        id: neutralPose
    }

    SkeletonPose {
        id: ragdollPose
    }

    SkeletonMapper {
        id: mapper
        ragdoll: sourceRagdoll
        skin: targetFigure.skin
        modelNode: targetFigure
        neutralPose: neutralPose
        lockTranslations: false
    }

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        forceDebugDraw: false
        running: AppSettings.playPhysics
        gravity: Qt.vector3d(0, -AppSettings.gravity, 0)
        numThreads: AppSettings.maxConcurrentJobs
        minimumTimestep: AppSettings.minimumTimestep
        maximumTimestep: AppSettings.maximumTimestep
        collisionSteps: AppSettings.collisionSteps
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        onBeforeFrameDone: tryInitializeMapper()
        onFrameDone: syncTargetFigure()
        settings: PhysicsSettings {
            numVelocitySteps: Math.round(AppSettings.numVelocitySteps)
            numPositionSteps: Math.round(AppSettings.numPositionSteps)
            baumgarte: AppSettings.baumgarte
            speculativeContactDistance: AppSettings.speculativeContactDistance
            penetrationSlop: AppSettings.penetrationSlop
            linearCastThreshold: AppSettings.linearCastThreshold
            minVelocityForRestitution: AppSettings.minVelocityForRestitution
            timeBeforeSleep: AppSettings.timeBeforeSleep
            pointVelocitySleepThreshold: AppSettings.pointVelocitySleepThreshold
            deterministicSimulation: AppSettings.deterministicSimulation
            constraintWarmStart: AppSettings.constraintWarmStart
            useBodyPairContactCache: AppSettings.useBodyPairContactCache
            useManifoldReduction: AppSettings.useManifoldReduction
            useLargeIslandSplitter: AppSettings.useLargeIslandSplitter
            allowSleeping: AppSettings.allowSleeping
            checkActiveEdges: AppSettings.checkActiveEdges
        }
    }

    SampleCameraController {
        camera: camera
        physicsSystem: physicsSystem
        speed: 0.02
        shiftSpeed: 0.06
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#191970"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(30, 10, 30).times(worldScale)
            clipFar: 50000
            clipNear: 0.01
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(-30, -10, -30).times(worldScale))
            }
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            castsShadow: true
            shadowFactor: 75
            shadowMapQuality: Light.ShadowMapQualityVeryHigh
            pcfFactor: 0.01
            shadowBias: 0.1
        }

        Body {
            shape: BoxShape {
                extents: Qt.vector3d(350 * worldScale, 2, 350 * worldScale)
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, -1, 0)
            activation: Body.DontActivate
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: "white"
                    metalness: 0.5
                    roughness: 0.1
                }
                castsShadows: false
                scale: Qt.vector3d(3.5 * worldScale, 0.02, 3.5 * worldScale)
            }
        }

        SimpleHumanRagdoll {
            id: sourceRagdoll
            objectLayer: moving
            visible: false
        }

        RiggedFigure {
            id: targetFigure
        }
    }

    SettingsView {}
}
