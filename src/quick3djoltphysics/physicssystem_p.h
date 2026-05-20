#ifndef PHYSICSSYSTEM_P_H
#define PHYSICSSYSTEM_P_H

#include "physicslayers.h"
#include "physicssettings_p.h"
#include "raycastresult_p.h"
#include "collideshaperesult_p.h"
#include "shapecastresult_p.h"
#include "triangleresult_p.h"
#include "abstractcontactlistener.h"
#include "abstractsoftbodycontactlistener.h"
#include "qtquick3djoltphysicsglobal_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QLoggingCategory>
#include <QObject>
#include <QPointer>
#include <QtQml/qqml.h>
#include <QAbstractAnimation>

#include <QtQuick3D/private/qquick3dviewport_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/BodyFilter.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

namespace JPH {
class PhysicsSystem;
class TempAllocator;
class JobSystem;
class Body;
}

Q_DECLARE_LOGGING_CATEGORY(lcQuick3dJoltPhysics)

class AbstractPhysicsNode;
class CharacterVirtual;
class BodyFilter;
class PhysicsSystemAnimation;
class PhysicsSystemUpdate;

class Q_QUICK3DJOLTPHYSICS_EXPORT PhysicsSystem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(int startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(PhysicsSettings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int collisionSteps READ collisionSteps WRITE setCollisionSteps NOTIFY collisionStepsChanged)
    Q_PROPERTY(quint32 numBodies READ numBodies WRITE setNumBodies NOTIFY numBodiesChanged)
    Q_PROPERTY(quint32 numBodyMutexes READ numBodyMutexes WRITE setNumBodyMutexes NOTIFY numBodyMutexesChanged)
    Q_PROPERTY(quint32 maxBodyPairs READ maxBodyPairs WRITE setMaxBodyPairs NOTIFY maxBodyPairsChanged)
    Q_PROPERTY(quint32 maxContactConstraints READ maxContactConstraints WRITE setMaxContactConstraints NOTIFY maxContactConstraintsChanged)
    Q_PROPERTY(int numThreads READ numThreads WRITE setNumThreads NOTIFY numThreadsChanged)
    Q_PROPERTY(AbstractObjectLayerPairFilter *objectLayerPairFilter READ objectLayerPairFilter WRITE
                   setObjectLayerPairFilter NOTIFY objectLayerPairFilterChanged)
    Q_PROPERTY(AbstractBroadPhaseLayer *broadPhaseLayer READ broadPhaseLayer WRITE
                   setBroadPhaseLayer NOTIFY broadPhaseLayerChanged)
    Q_PROPERTY(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter READ objectVsBroadPhaseLayerFilter WRITE
                   setObjectVsBroadPhaseLayerFilter NOTIFY objectVsBroadPhaseLayerFilterChanged)
    Q_PROPERTY(AbstractContactListener *contactListener READ contactListener WRITE setContactListener NOTIFY contactListenerChanged)
    Q_PROPERTY(AbstractSoftBodyContactListener *softBodyContactListener READ softBodyContactListener WRITE setSoftBodyContactListener NOTIFY softBodyContactListenerChanged)
    Q_PROPERTY(QQuick3DNode *scene READ scene WRITE setScene NOTIFY sceneChanged)

    QML_NAMED_ELEMENT(PhysicsSystem)
public:
    PhysicsSystem(QObject *parent = nullptr);
    ~PhysicsSystem();

    void classBegin() override {}
    void componentComplete() override;

    static PhysicsSystem *getPhysicsSystem(QQuick3DNode *node);

    static void registerPhysicsNode(AbstractPhysicsNode *physicsNode);
    static void deregisterPhysicsNode(AbstractPhysicsNode *physicsNode);

    static void addToCharacterVsCharacterCollision(CharacterVirtual *character);
    static void removeFromCharacterVsCharacterCollision(CharacterVirtual *character);

    int startTime() const;
    void setStartTime(int startTime);
    int time() const;
    void setTime(int time);
    PhysicsSettings *settings() const;
    void setSettings(PhysicsSettings *settings);
    QVector3D gravity() const;
    void setGravity(QVector3D gravity);
    bool running() const;
    void setRunning(bool running);
    int collisionSteps() const;
    void setCollisionSteps(int collisionSteps);
    quint32 numBodies() const;
    void setNumBodies(quint32 maxBodies);
    quint32 numBodyMutexes() const;
    void setNumBodyMutexes(quint32 numBodyMutexes);
    quint32 maxBodyPairs() const;
    void setMaxBodyPairs(quint32 maxBodyPairs);
    quint32 maxContactConstraints() const;
    void setMaxContactConstraints(quint32 maxContactConstraints);
    int numThreads() const;
    void setNumThreads(int numThreads);
    AbstractObjectLayerPairFilter *objectLayerPairFilter() const;
    void setObjectLayerPairFilter(AbstractObjectLayerPairFilter *objectLayerPairFilter);
    AbstractBroadPhaseLayer *broadPhaseLayer() const;
    void setBroadPhaseLayer(AbstractBroadPhaseLayer *broadPhaseLayer);
    AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter() const;
    void setObjectVsBroadPhaseLayerFilter(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter);
    AbstractContactListener *contactListener() const;
    void setContactListener(AbstractContactListener *contactListener);
    AbstractSoftBodyContactListener *softBodyContactListener() const;
    void setSoftBodyContactListener(AbstractSoftBodyContactListener *listener);
    QQuick3DNode *scene() const;
    void setScene(QQuick3DNode *scene);

    Q_INVOKABLE RayCastResult castRay(const QVector3D &origin, const QVector3D &direction, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE RayCastResult castRay(const QVector3D &origin, const QVector3D &direction, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE RayCastResult castRay(const QVector3D &origin, const QVector3D &direction, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<RayCastResult> collectRay(const QVector3D &origin, const QVector3D &direction, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<RayCastResult> collectRay(const QVector3D &origin, const QVector3D &direction, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<RayCastResult> collectRay(const QVector3D &origin, const QVector3D &direction, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<Body *> collidePoint(const QVector3D &point, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<Body *> collidePoint(const QVector3D &point, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<Body *> collidePoint(const QVector3D &point, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<CollideShapeResult> collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<CollideShapeResult> collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<CollideShapeResult> collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<ShapeCastResult> castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<ShapeCastResult> castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<ShapeCastResult> castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits = 0) const;
    Q_INVOKABLE QVector<TriangleResult> getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<TriangleResult> getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;
    Q_INVOKABLE QVector<TriangleResult> getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const;

signals:
    void startTimeChanged(int startTime);
    void timeChanged(int time);
    void settingsChanged(PhysicsSettings *settings);
    void gravityChanged(QVector3D gravity);
    void runningChanged(bool running);
    void collisionStepsChanged(int collisionSteps);
    void numBodiesChanged(quint32 maxBodies);
    void numBodyMutexesChanged(quint32 numBodyMutexes);
    void maxBodyPairsChanged(quint32 maxBodyPairs);
    void maxContactConstraintsChanged(quint32 maxContactConstraints);
    void numThreadsChanged(int numThreads);
    void objectLayerPairFilterChanged(AbstractObjectLayerPairFilter *objectLayerPairFilter);
    void broadPhaseLayerChanged(AbstractBroadPhaseLayer *broadPhaseLayer);
    void objectVsBroadPhaseLayerFilterChanged(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter);
    void contactListenerChanged(AbstractContactListener *contactListener);
    void softBodyContactListenerChanged(AbstractSoftBodyContactListener *listener);
    void sceneChanged(QQuick3DNode *scene);
    void updateFrame(float frequency, int collisionSteps);
    void beforeFrameDone(float deltaTime);
    void frameDone(float deltaTime);

private:
    void initPhysics();
    void matchOrphanPhysicsNodes();
    void findPhysicsNodes();
    void updateCurrentTime(int currentTime);
    void refresh();
    void emitContactCallbacks();

    friend class PhysicsSystemAnimation;
    friend class PhysicsSystemUpdate;

    QList<AbstractPhysicsNode *> m_physicsNodes;

    int m_startTime = 0;
    int m_time = 0;

    int m_currentTime = 0;

    PhysicsSettings *m_settings = nullptr;
    bool m_settingsDirty = false;
    QVector3D m_gravity = QVector3D(0.f, -981.f, 0.f);
    bool m_running = true;
    int m_collisionSteps = 1;
    quint32 m_numBodies = 10240;
    quint32 m_numBodyMutexes = 0;
    quint32 m_maxBodyPairs = 65536;
    quint32 m_maxContactConstraints = 20480;
    int m_numThreads = -1;
    QQuick3DNode *m_scene = nullptr;

    JPH::PhysicsSystem *m_jolt = nullptr;
    JPH::TempAllocator *m_tempAllocator = nullptr;
    JPH::JobSystem *m_jobSystem = nullptr;
    JPH::JobSystem *m_jobSystemValidating = nullptr;
    AbstractObjectLayerPairFilter *m_objectLayerPairFilter = nullptr;
    AbstractBroadPhaseLayer *m_broadPhaseLayer = nullptr;
    AbstractObjectVsBroadPhaseLayerFilter *m_objectVsBroadPhaseLayerFilter = nullptr;
    AbstractContactListener *m_contactListener = nullptr;
    AbstractSoftBodyContactListener *m_softBodyContactListener = nullptr;

    JPH::CharacterVsCharacterCollisionSimple m_characterVsCharacterCollision;

    PhysicsSystemAnimation *m_animation = nullptr;
    PhysicsSystemUpdate *m_updateAnimation = nullptr;
    bool m_physicsInitialized = false;
};

class PhysicsSystemAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    PhysicsSystemAnimation(PhysicsSystem *system)
        : QAbstractAnimation(static_cast<QObject *>(system)), m_system(system)
    {}
protected:
    void updateCurrentTime(int t) override
    {
        m_system->updateCurrentTime(t + m_system->startTime());
    }

    int duration() const override
    {
        return -1;
    }

private:
    PhysicsSystem *m_system = nullptr;
};

class PhysicsSystemUpdate : public QAbstractAnimation
{
    Q_OBJECT
public:
    PhysicsSystemUpdate(PhysicsSystem *system)
        : QAbstractAnimation(static_cast<QObject *>(system)), m_system(system)
    {}

    void setDirty(bool dirty)
    {
        m_dirty = dirty;
    }

protected:
    void updateCurrentTime(int t) override
    {
        Q_UNUSED(t);
        if (m_dirty)
            m_system->refresh();
    }

    int duration() const override
    {
        return -1;
    }
private:
    PhysicsSystem *m_system = nullptr;
    bool m_dirty = false;
};

#endif // PHYSICSSYSTEM_P_H
