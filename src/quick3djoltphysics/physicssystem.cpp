#include "physicssystem_p.h"

#include "abstractphysicsnode_p.h"
#include "charactervirtual_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/CollidePointResult.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/ShapeCast.h>

Q_LOGGING_CATEGORY(lcQuick3dJoltPhysics, "qt.quick3d.joltphysics")

int g_factoryRefCount = 0;

struct PhysicsSystemManager
{
    QVector<PhysicsSystem *> physicsSystems;
    QVector<AbstractPhysicsNode *> orphanPhysicsNodes;
};

static PhysicsSystemManager physicsSystemManager = PhysicsSystemManager {};

PhysicsSystem::PhysicsSystem(QObject *parent)
    : QObject(parent)
    , m_animation(new PhysicsSystemAnimation(this))
    , m_updateAnimation(new PhysicsSystemUpdate(this))
{
    if (g_factoryRefCount == 0) {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory;
        JPH::RegisterTypes();
    }
    g_factoryRefCount++;
    m_tempAllocator = new JPH::TempAllocatorImpl(32 * 1024 * 1024);

    physicsSystemManager.physicsSystems.append(this);
}

PhysicsSystem::~PhysicsSystem()
{
    m_animation->stop();
    m_updateAnimation->stop();

    for (auto *physicsNode : std::as_const(m_physicsNodes))
        physicsNode->cleanup();

    m_physicsNodes.clear();

    g_factoryRefCount--;
    if (g_factoryRefCount == 0) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    delete m_jolt;
    m_jolt = nullptr;
    delete m_tempAllocator;
    m_tempAllocator = nullptr;

    physicsSystemManager.physicsSystems.removeAll(this);
}

void PhysicsSystem::componentComplete()
{
    if (!m_physicsInitialized)
        initPhysics();

    m_updateAnimation->start();

    if (m_animation->state() == QAbstractAnimation::Running)
        m_animation->stop();
    if (m_running)
        m_animation->start();

    m_time = 0;
    m_currentTime = 0;

    emit timeChanged(m_time);

    matchOrphanPhysicsNodes();
}

PhysicsSystem *PhysicsSystem::getPhysicsSystem(QQuick3DNode *node)
{
    for (auto *physicsSystem : physicsSystemManager.physicsSystems) {
        if (!physicsSystem->m_scene)
            continue;

        auto *nodeCurr = node;

        if (nodeCurr == physicsSystem->m_scene)
            return physicsSystem;

        while (nodeCurr->parentNode()) {
            nodeCurr = nodeCurr->parentNode();
            if (nodeCurr == physicsSystem->m_scene)
                return physicsSystem;
        }
    }

    return nullptr;
}

void PhysicsSystem::registerPhysicsNode(AbstractPhysicsNode *physicsNode)
{
    auto *physicsSystem = getPhysicsSystem(physicsNode);
    if (physicsSystem && physicsSystem->m_jolt) {
        if (!physicsSystem->m_physicsNodes.contains(physicsNode))
            physicsSystem->m_physicsNodes.push_back(physicsNode);
        physicsNode->init(physicsSystem->m_jolt, physicsSystem->m_tempAllocator);
    } else {
        physicsSystemManager.orphanPhysicsNodes.push_back(physicsNode);
    }
}

void PhysicsSystem::deregisterPhysicsNode(AbstractPhysicsNode *physicsNode)
{
    for (auto *physicsSystem : std::as_const(physicsSystemManager.physicsSystems))
        physicsSystem->m_physicsNodes.removeAll(physicsNode);
    physicsSystemManager.orphanPhysicsNodes.removeAll(physicsNode);
}

void PhysicsSystem::addToCharacterVsCharacterCollision(CharacterVirtual *character)
{
    auto *physicsSystem = getPhysicsSystem(character);
    if (physicsSystem) {
        character->m_character->SetCharacterVsCharacterCollision(
            &physicsSystem->m_characterVsCharacterCollision);
        physicsSystem->m_characterVsCharacterCollision.Add(character->m_character);
    } else {
        qWarning() << "No physics system found, cannot add to character vs character collision.";
    }
}

void PhysicsSystem::removeFromCharacterVsCharacterCollision(CharacterVirtual *character)
{
    for (auto *physicsSystem : std::as_const(physicsSystemManager.physicsSystems))
        physicsSystem->m_characterVsCharacterCollision.Remove(character->m_character);
}

int PhysicsSystem::startTime() const
{
    return m_startTime;
}

void PhysicsSystem::setStartTime(int startTime)
{
    if (m_startTime == startTime)
        return;

    m_startTime = startTime;
    emit startTimeChanged(m_startTime);
}

int PhysicsSystem::time() const
{
    return m_time;
}

void PhysicsSystem::setTime(int time)
{
    if (m_time == time)
        return;

    m_time = time;
    m_updateAnimation->setDirty(true);
    emit timeChanged(m_time);
}

PhysicsSettings *PhysicsSystem::settings() const
{
    return m_settings;
}

void PhysicsSystem::setSettings(PhysicsSettings *settings)
{
    if (m_settings == settings)
        return;

    if (m_settings)
        m_settings->disconnect(this);

    m_settings = settings;

    if (m_settings) {
        QObject::connect(m_settings, &PhysicsSettings::changed, this,
                         [this] { m_settingsDirty = true; });
        QObject::connect(m_settings, &QObject::destroyed, this,
                         [this](QObject *obj)
                         {
                             if (m_settings == obj)
                                 setSettings(nullptr);
                         });
    }

    m_settingsDirty = true;
    emit settingsChanged(m_settings);
}

QVector3D PhysicsSystem::gravity() const
{
    return m_gravity;
}

void PhysicsSystem::setGravity(QVector3D gravity)
{
    if (m_gravity == gravity)
        return;

    m_gravity = gravity;
    if (m_physicsInitialized)
        m_jolt->SetGravity(PhysicsUtils::toJoltType(m_gravity));

    emit gravityChanged(m_gravity);
}

bool PhysicsSystem::running() const
{
    return m_running;
}

void PhysicsSystem::setRunning(bool running)
{
    if (m_running == running)
        return;
    m_running = running;
    if (m_running)
        m_animation->start();
    else
        m_animation->stop();

    emit runningChanged(m_running);
}

int PhysicsSystem::collisionSteps() const
{
    return m_collisionSteps;
}

void PhysicsSystem::setCollisionSteps(int collisionSteps)
{
    if (m_collisionSteps == collisionSteps)
        return;

    m_collisionSteps = collisionSteps;
    emit collisionStepsChanged(m_collisionSteps);
}

quint32 PhysicsSystem::numBodies() const
{
    return m_numBodies;
}

void PhysicsSystem::setNumBodies(quint32 maxBodies)
{
    if (m_numBodies == maxBodies)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxBodies' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numBodies = maxBodies;

    emit numBodiesChanged(maxBodies);
}

quint32 PhysicsSystem::numBodyMutexes() const
{
    return m_numBodyMutexes;
}

void PhysicsSystem::setNumBodyMutexes(quint32 numBodyMutexes)
{
    if (m_numBodyMutexes == numBodyMutexes)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'numBodyMutexes' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numBodyMutexes = numBodyMutexes;
    emit numBodyMutexesChanged(numBodyMutexes);
}

quint32 PhysicsSystem::maxBodyPairs() const
{
    return m_maxBodyPairs;
}

void PhysicsSystem::setMaxBodyPairs(quint32 maxBodyPairs)
{
    if (m_maxBodyPairs == maxBodyPairs)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxBodyPairs' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_maxBodyPairs = maxBodyPairs;
    emit maxBodyPairsChanged(maxBodyPairs);
}

quint32 PhysicsSystem::maxContactConstraints() const
{
    return m_maxContactConstraints;
}

void PhysicsSystem::setMaxContactConstraints(quint32 maxContactConstraints)
{
    if (m_maxContactConstraints == maxContactConstraints)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxContactConstraints' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_maxContactConstraints = maxContactConstraints;
    emit maxContactConstraintsChanged(maxContactConstraints);
}

int PhysicsSystem::numThreads() const
{
    return m_numThreads;
}

void PhysicsSystem::setNumThreads(int numThreads)
{
    if (m_numThreads == numThreads)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'numThreads' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numThreads = numThreads;
    emit numThreadsChanged(numThreads);
}

AbstractObjectLayerPairFilter *PhysicsSystem::objectLayerPairFilter() const
{
    return m_objectLayerPairFilter;
}

void PhysicsSystem::setObjectLayerPairFilter(AbstractObjectLayerPairFilter *objectLayerPairFilter)
{
    if (m_objectLayerPairFilter == objectLayerPairFilter)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'objectLayerPairFilter' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_objectLayerPairFilter = objectLayerPairFilter;
    emit objectLayerPairFilterChanged(objectLayerPairFilter);
}

AbstractBroadPhaseLayer *PhysicsSystem::broadPhaseLayer() const
{
    return m_broadPhaseLayer;
}

void PhysicsSystem::setBroadPhaseLayer(AbstractBroadPhaseLayer *broadPhaseLayer)
{
    if (m_broadPhaseLayer == broadPhaseLayer)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'broadPhaseLayer' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_broadPhaseLayer = broadPhaseLayer;
    emit broadPhaseLayerChanged(broadPhaseLayer);
}

AbstractObjectVsBroadPhaseLayerFilter *PhysicsSystem::objectVsBroadPhaseLayerFilter() const
{
    return m_objectVsBroadPhaseLayerFilter;
}

void PhysicsSystem::setObjectVsBroadPhaseLayerFilter(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter)
{
    if (m_objectVsBroadPhaseLayerFilter == objectVsBroadPhaseLayerFilter)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'objectVsBroadPhaseLayerFilter' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_objectVsBroadPhaseLayerFilter = objectVsBroadPhaseLayerFilter;
    emit objectVsBroadPhaseLayerFilterChanged(objectVsBroadPhaseLayerFilter);
}

AbstractContactListener *PhysicsSystem::contactListener() const
{
    return m_contactListener;
}

void PhysicsSystem::setContactListener(AbstractContactListener *contactListener)
{
    if (m_contactListener == contactListener)
        return;

    if (m_contactListener)
        m_contactListener->disconnect(this);

    m_contactListener = contactListener;

    if (m_contactListener) {
        QObject::connect(m_contactListener, &QObject::destroyed, this,
                         [this](QObject *obj)
                         {
                             if (m_contactListener == obj)
                                 setContactListener(nullptr);
                         });
    }

    if (m_jolt) {
        if (m_contactListener)
            m_jolt->SetContactListener(m_contactListener->getJoltContactListener());
        else
            m_jolt->SetContactListener(nullptr);
    }

    emit contactListenerChanged(m_contactListener);
}

AbstractSoftBodyContactListener *PhysicsSystem::softBodyContactListener() const
{
    return m_softBodyContactListener;
}

void PhysicsSystem::setSoftBodyContactListener(AbstractSoftBodyContactListener *listener)
{
    if (m_softBodyContactListener == listener)
        return;

    if (m_softBodyContactListener)
        m_softBodyContactListener->disconnect(this);

    m_softBodyContactListener = listener;

    if (m_softBodyContactListener) {
        QObject::connect(m_softBodyContactListener, &QObject::destroyed, this,
                         [this](QObject *obj)
                         {
                             if (m_softBodyContactListener == obj)
                                 setSoftBodyContactListener(nullptr);
                         });
    }

    if (m_jolt) {
        if (m_softBodyContactListener)
            m_jolt->SetSoftBodyContactListener(m_softBodyContactListener->getJoltSoftBodyContactListener());
        else
            m_jolt->SetSoftBodyContactListener(nullptr);
    }

    emit softBodyContactListenerChanged(m_softBodyContactListener);
}

QQuick3DNode *PhysicsSystem::scene() const
{
    return m_scene;
}

void PhysicsSystem::setScene(QQuick3DNode *scene)
{
    if (m_scene == scene)
        return;

    m_scene = scene;

    if (m_physicsInitialized) {
        auto physicsNodes = m_physicsNodes;
        for (auto node : std::as_const(physicsNodes))
            deregisterPhysicsNode(node);

        m_physicsNodes.clear();

        bool sceneOK = true;
        for (auto *physicsSystem : physicsSystemManager.physicsSystems) {
            if (physicsSystem != this && physicsSystem->scene() == scene) {
                sceneOK = false;
                qWarning() << "Warning: scene already associated with physics world";
            }
        }

        if (sceneOK)
            findPhysicsNodes();
    }

    emit sceneChanged(m_scene);
}

static RayCastResult castRay_helper(JPH::PhysicsSystem *jolt, const QVector3D &origin, const QVector3D &direction, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    JPH::RRayCast ray {
        PhysicsUtils::toJoltType(origin),
        PhysicsUtils::toJoltType(direction)
    };

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    JPH::RayCastResult hit;
    bool hadHit = jolt->GetNarrowPhaseQuery().CastRay(ray, hit, broadPhaseLayerFilter, objectLayerFilter, bf);

    Body *body = nullptr;
    JPH::Vec3 position = ray.GetPointOnRay(hit.mFraction);
    JPH::Vec3 normal;

    if (hadHit) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            body = reinterpret_cast<Body *>(joltBody.GetUserData());
            normal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, position);
        }
    }

    return RayCastResult(
        body,
        body != nullptr ? PhysicsUtils::toQtType(position) : QVector3D(),
        body != nullptr ? PhysicsUtils::toQtType(normal) : QVector3D(),
        hit.mFraction);
}

static QVector<RayCastResult> collectRay_helper(JPH::PhysicsSystem *jolt, const QVector3D &origin, const QVector3D &direction, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits)
{
    JPH::RRayCast ray {
        PhysicsUtils::toJoltType(origin),
        PhysicsUtils::toJoltType(direction)
    };

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    JPH::RayCastSettings settings;
    JPH::Array<JPH::RayCastResult> hits;

    if (maxHits == 0) {
        JPH::AnyHitCollisionCollector<JPH::CastRayCollector> collector;
        jolt->GetNarrowPhaseQuery().CastRay(ray, settings, collector, broadPhaseLayerFilter, objectLayerFilter, bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else if (maxHits == 1) {
        JPH::ClosestHitCollisionCollector<JPH::CastRayCollector> collector;
        jolt->GetNarrowPhaseQuery().CastRay(ray, settings, collector, broadPhaseLayerFilter, objectLayerFilter, bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else {
        JPH::AllHitCollisionCollector<JPH::CastRayCollector> collector;
        jolt->GetNarrowPhaseQuery().CastRay(ray, settings, collector, broadPhaseLayerFilter, objectLayerFilter, bf);
        collector.Sort();
        hits.insert(hits.end(), collector.mHits.begin(), collector.mHits.end());
        if ((int)hits.size() > maxHits)
            hits.resize(maxHits);
    }

    QVector<RayCastResult> results;

    for (const auto &hit : hits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            JPH::Vec3 position = ray.GetPointOnRay(hit.mFraction);
            JPH::Vec3 normal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, position);
            results.push_back(RayCastResult(
                body,
                body != nullptr ? PhysicsUtils::toQtType(position) : QVector3D(),
                body != nullptr ? PhysicsUtils::toQtType(normal) : QVector3D(),
                hit.mFraction));
        }
    }

    return results;
}

static QVector<Body *> collidePoint_helper(JPH::PhysicsSystem *jolt, const QVector3D &point, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    JPH::AllHitCollisionCollector<JPH::CollidePointCollector> collector;
    jolt->GetNarrowPhaseQuery().CollidePoint(PhysicsUtils::toJoltType(point), collector, broadPhaseLayerFilter, objectLayerFilter, bf);

    QVector<Body *> hits;
    for (const auto &hit : collector.mHits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            hits.push_back(body);
        }
    }

    return hits;
}

static QVector<CollideShapeResult> collideShape_helper(JPH::PhysicsSystem *jolt, const JPH::Ref<JPH::Shape> &shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits)
{
    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    JPH::CollideShapeSettings settings;
    JPH::Array<JPH::CollideShapeResult> hits;

    if (maxHits == 0) {
        JPH::AnyHitCollisionCollector<JPH::CollideShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CollideShape(shape,
                                                 JPH::Vec3::sReplicate(1.0f),
                                                 PhysicsUtils::toJoltType(transform),
                                                 settings,
                                                 PhysicsUtils::toJoltType(baseOffset),
                                                 collector,
                                                 broadPhaseLayerFilter,
                                                 objectLayerFilter,
                                                 bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else if (maxHits == 1) {
        JPH::ClosestHitCollisionCollector<JPH::CollideShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CollideShape(shape,
                                                 JPH::Vec3::sReplicate(1.0f),
                                                 PhysicsUtils::toJoltType(transform),
                                                 settings,
                                                 PhysicsUtils::toJoltType(baseOffset),
                                                 collector,
                                                 broadPhaseLayerFilter,
                                                 objectLayerFilter,
                                                 bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else {
        JPH::AllHitCollisionCollector<JPH::CollideShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CollideShape(shape,
                                                 JPH::Vec3::sReplicate(1.0f),
                                                 PhysicsUtils::toJoltType(transform),
                                                 settings,
                                                 PhysicsUtils::toJoltType(baseOffset),
                                                 collector,
                                                 broadPhaseLayerFilter,
                                                 objectLayerFilter,
                                                 bf);
        collector.Sort();
        hits.insert(hits.end(), collector.mHits.begin(), collector.mHits.end());
        if ((int)hits.size() > maxHits)
            hits.resize(maxHits);
    }

    QVector<CollideShapeResult> results;

    for (const auto &hit : hits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID2);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            const auto &surfaceNormal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, hit.mContactPointOn2);
            results.push_back(CollideShapeResult(PhysicsUtils::toQtType(hit.mContactPointOn1),
                                                 PhysicsUtils::toQtType(hit.mContactPointOn2),
                                                 PhysicsUtils::toQtType(hit.mPenetrationAxis),
                                                 hit.mPenetrationDepth,
                                                 PhysicsUtils::toQtType(surfaceNormal),
                                                 body));
        }
    }

    return results;
}

static QVector<ShapeCastResult> castShape_helper(JPH::PhysicsSystem *jolt, const JPH::Ref<JPH::Shape> &shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits)
{
    JPH::RShapeCast shapeCast = JPH::RShapeCast::sFromWorldTransform(shape,
                                                                     JPH::Vec3::sReplicate(1.0f),
                                                                     PhysicsUtils::toJoltType(transform),
                                                                     PhysicsUtils::toJoltType(direction));

    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    JPH::ShapeCastSettings settings;
    JPH::Array<JPH::ShapeCastResult> hits;

    if (maxHits == 0) {
        JPH::AnyHitCollisionCollector<JPH::CastShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CastShape(shapeCast,
                                              settings,
                                              PhysicsUtils::toJoltType(baseOffset),
                                              collector,
                                              broadPhaseLayerFilter,
                                              objectLayerFilter,
                                              bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else if (maxHits == 1) {
        JPH::ClosestHitCollisionCollector<JPH::CastShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CastShape(shapeCast,
                                              settings,
                                              PhysicsUtils::toJoltType(baseOffset),
                                              collector,
                                              broadPhaseLayerFilter,
                                              objectLayerFilter,
                                              bf);
        if (collector.HadHit())
            hits.push_back(collector.mHit);
    } else {
        JPH::AllHitCollisionCollector<JPH::CastShapeCollector> collector;
        jolt->GetNarrowPhaseQuery().CastShape(shapeCast,
                                              settings,
                                              PhysicsUtils::toJoltType(baseOffset),
                                              collector,
                                              broadPhaseLayerFilter,
                                              objectLayerFilter,
                                              bf);
        collector.Sort();
        hits.insert(hits.end(), collector.mHits.begin(), collector.mHits.end());
        if ((int)hits.size() > maxHits)
            hits.resize(maxHits);
    }

    QVector<ShapeCastResult> results;

    for (const auto &hit : hits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID2);
        if (lock.Succeeded()) {
            const auto &position = shapeCast.GetPointOnRay(hit.mFraction);
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            const auto &surfaceNormal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, hit.mContactPointOn2);
            results.push_back(ShapeCastResult(PhysicsUtils::toQtType(hit.mContactPointOn1),
                                              PhysicsUtils::toQtType(hit.mContactPointOn2),
                                              PhysicsUtils::toQtType(hit.mPenetrationAxis),
                                              hit.mPenetrationDepth,
                                              PhysicsUtils::toQtType(surfaceNormal),
                                              body,
                                              PhysicsUtils::toQtType(position),
                                              hit.mFraction,
                                              hit.mIsBackFaceHit));
        }
    }

    return results;
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return castRay_helper(m_jolt,
                          origin,
                          direction,
                          {},
                          {},
                          bodyFilter);
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return castRay_helper(m_jolt,
                          origin,
                          direction,
                          m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                          m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                          bodyFilter);
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return castRay_helper(m_jolt,
                          origin,
                          direction,
                          JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                          JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                          bodyFilter);
}

QVector<RayCastResult> PhysicsSystem::collectRay(const QVector3D &origin, const QVector3D &direction, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collectRay_helper(m_jolt,
                             origin,
                             direction,
                             {},
                             {},
                             bodyFilter,
                             maxHits);
}

QVector<RayCastResult> PhysicsSystem::collectRay(const QVector3D &origin, const QVector3D &direction, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collectRay_helper(m_jolt,
                             origin,
                             direction,
                             m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                             m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                             bodyFilter,
                             maxHits);
}

QVector<RayCastResult> PhysicsSystem::collectRay(const QVector3D &origin, const QVector3D &direction, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collectRay_helper(m_jolt,
                             origin,
                             direction,
                             JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                             JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                             bodyFilter,
                             maxHits);
}

QVector<Body *> PhysicsSystem::collidePoint(const QVector3D &point, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return collidePoint_helper(m_jolt,
                               point,
                               {},
                               {},
                               bodyFilter);
}

QVector<Body *> PhysicsSystem::collidePoint(const QVector3D &point, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return collidePoint_helper(m_jolt,
                               point,
                               m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                               m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                               bodyFilter);
}

QVector<Body *> PhysicsSystem::collidePoint(const QVector3D &point, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return collidePoint_helper(m_jolt,
                               point,
                               JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                               JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                               bodyFilter);
}

QVector<CollideShapeResult> PhysicsSystem::collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collideShape_helper(m_jolt,
                               shape->getJoltShape(),
                               transform,
                               baseOffset,
                               {},
                               {},
                               bodyFilter,
                               maxHits);
}

QVector<CollideShapeResult> PhysicsSystem::collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collideShape_helper(m_jolt,
                               shape->getJoltShape(),
                               transform,
                               baseOffset,
                               m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                               m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                               bodyFilter,
                               maxHits);
}

QVector<CollideShapeResult> PhysicsSystem::collideShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return collideShape_helper(m_jolt,
                               shape->getJoltShape(),
                               transform,
                               baseOffset,
                               JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                               JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                               bodyFilter,
                               maxHits);
}

QVector<ShapeCastResult> PhysicsSystem::castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return castShape_helper(m_jolt,
                            shape->getJoltShape(),
                            transform,
                            direction,
                            baseOffset,
                            {},
                            {},
                            bodyFilter,
                            maxHits);
}

QVector<ShapeCastResult> PhysicsSystem::castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return castShape_helper(m_jolt,
                            shape->getJoltShape(),
                            transform,
                            direction,
                            baseOffset,
                            m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                            m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                            bodyFilter,
                            maxHits);
}

QVector<ShapeCastResult> PhysicsSystem::castShape(AbstractShape *shape, const QMatrix4x4 &transform, const QVector3D &direction, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter, int maxHits) const
{
    return castShape_helper(m_jolt,
                            shape->getJoltShape(),
                            transform,
                            direction,
                            baseOffset,
                            JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                            JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                            bodyFilter,
                            maxHits);
}

static QVector<TriangleResult> getTriangles_helper(JPH::PhysicsSystem *jolt, const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, const JPH::BroadPhaseLayerFilter &broadPhaseLayerFilter, const JPH::ObjectLayerFilter &objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter)
{
    JPH::IgnoreMultipleBodiesFilter bf;
    for (const auto &body : std::as_const(bodyFilter)) {
        if (body != nullptr)
            bf.IgnoreBody(JPH::BodyID(body->bodyID()));
    }

    const JPH::Vec3 &min = PhysicsUtils::toJoltType(position - scale / 2);
    const JPH::Vec3 &max = PhysicsUtils::toJoltType(position + scale / 2);
    JPH::AABox box(min, max);

    JPH::AllHitCollisionCollector<JPH::TransformedShapeCollector> collector;

    jolt->GetNarrowPhaseQuery().CollectTransformedShapes(box, collector, broadPhaseLayerFilter, objectLayerFilter, bf);

    constexpr int maxTriangles = 32;
    QVector<TriangleResult> allTriangles;
    for (const JPH::TransformedShape &ts : collector.mHits) {
        JPH::Float3 vertices[maxTriangles * 3];
        JPH::Shape::GetTrianglesContext ctx;

        Body *qtBody = nullptr;
        JPH::BodyLockRead bodyLock(jolt->GetBodyLockInterface(), JPH::BodyID(ts.mBodyID));
        if (bodyLock.Succeeded()) {
            const JPH::Body &body = bodyLock.GetBody();
            qtBody = reinterpret_cast<Body *>(body.GetUserData());
        }

        ts.GetTrianglesStart(ctx, box, PhysicsUtils::toJoltType(baseOffset));
        while (true) {
            int count = ts.GetTrianglesNext(ctx, maxTriangles, vertices, nullptr);
            if (count == 0)
                break;

            for (int i = 0; i < count; ++i) {
                const auto &v0 = vertices[i * 3];
                const auto &v1 = vertices[i * 3 + 1];
                const auto &v2 = vertices[i * 3 + 2];
                allTriangles.push_back(TriangleResult(
                    baseOffset + QVector3D(v0.x, v0.y, v0.z),
                    baseOffset + QVector3D(v1.x, v1.y, v1.z),
                    baseOffset + QVector3D(v2.x, v2.y, v2.z),
                    qtBody));
            }
        }
    }

    return allTriangles;
}

QVector<TriangleResult> PhysicsSystem::getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return getTriangles_helper(m_jolt,
                               position,
                               scale,
                               baseOffset,
                               {},
                               {},
                               bodyFilter);
}

QVector<TriangleResult> PhysicsSystem::getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return getTriangles_helper(m_jolt,
                               position,
                               scale,
                               baseOffset,
                               m_jolt->GetDefaultBroadPhaseLayerFilter(objectLayerFilter),
                               m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                               bodyFilter);
}

QVector<TriangleResult> PhysicsSystem::getTriangles(const QVector3D &position, const QVector3D &scale, const QVector3D &baseOffset, quint32 broadPhaseLayerFilter, quint32 objectLayerFilter, const QVector<AbstractPhysicsBody *> &bodyFilter) const
{
    return getTriangles_helper(m_jolt,
                               position,
                               scale,
                               baseOffset,
                               JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                               JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)),
                               bodyFilter);
}

void PhysicsSystem::initPhysics()
{
    if (m_objectLayerPairFilter == nullptr) {
        qWarning() << "Warning: Object 'objectLayerPairFilter' must be provided";
        return;
    }

    if (m_broadPhaseLayer == nullptr) {
        qWarning() << "Warning: Object 'broadPhaseLayer' must be provided";
        return;
    }

    if (m_objectVsBroadPhaseLayerFilter == nullptr) {
        qWarning() << "Warning: Object 'objectVsBroadPhaseLayerFilter must be provided";
        return;
    }

    const quint32 numThreads = m_numThreads >= 0 ? m_numThreads : qMax(0, QThread::idealThreadCount());
    m_jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, numThreads);

    m_jolt = new JPH::PhysicsSystem();

    m_jolt->Init(m_numBodies,
                 m_numBodyMutexes,
                 m_maxBodyPairs,
                 m_maxContactConstraints,
                 m_broadPhaseLayer->impl(),
                 m_objectVsBroadPhaseLayerFilter->impl(),
                 m_objectLayerPairFilter->impl());

    if (m_settings)
        m_jolt->SetPhysicsSettings(m_settings->getJoltPhysicsSettings());

    if (m_contactListener)
        m_jolt->SetContactListener(m_contactListener->getJoltContactListener());

    if (m_softBodyContactListener)
        m_jolt->SetSoftBodyContactListener(m_softBodyContactListener->getJoltSoftBodyContactListener());

    m_jolt->SetGravity(PhysicsUtils::toJoltType(m_gravity));

    m_physicsInitialized = true;
}

void PhysicsSystem::matchOrphanPhysicsNodes()
{
    if (physicsSystemManager.orphanPhysicsNodes.isEmpty())
        return;

    qsizetype numNodes = physicsSystemManager.orphanPhysicsNodes.length();
    qsizetype idx = 0;

    while (idx < numNodes) {
        auto node = physicsSystemManager.orphanPhysicsNodes[idx];
        auto physicsSystem = getPhysicsSystem(node);
        if (physicsSystem == this) {
            physicsSystem->m_physicsNodes.push_back(node);
            node->init(m_jolt, m_tempAllocator);
            // swap-erase
            physicsSystemManager.orphanPhysicsNodes.swapItemsAt(idx, numNodes - 1);
            physicsSystemManager.orphanPhysicsNodes.pop_back();
            numNodes--;
        } else {
            idx++;
        }
    }
}

void PhysicsSystem::findPhysicsNodes()
{
    if (m_scene == nullptr)
        return;

    QList<QQuick3DObject *> children = m_scene->childItems();
    while (!children.empty()) {
        auto child = children.takeFirst();
        if (auto converted = qobject_cast<AbstractPhysicsNode *>(child); converted != nullptr) {
            if (!m_physicsNodes.contains(converted))
                m_physicsNodes.push_back(converted);
            converted->init(m_jolt, m_tempAllocator);
            physicsSystemManager.orphanPhysicsNodes.removeAll(converted);
        }
        children.append(child->childItems());
    }
}

void PhysicsSystem::updateCurrentTime(int currentTime)
{
    int dt = m_currentTime;
    m_currentTime = currentTime;
    int time = m_currentTime;

    dt = time - dt;

    float deltaTime = dt / 1000.0f;
    if (deltaTime < 0.0f)
        return;

    matchOrphanPhysicsNodes();
    emitContactCallbacks();

    emit beforeFrameDone(deltaTime);

    if (m_settingsDirty) {
        m_settingsDirty = false;
        m_jolt->SetPhysicsSettings(m_settings ? m_settings->getJoltPhysicsSettings() : JPH::PhysicsSettings());
    }

    QHash<QQuick3DNode *, QMatrix4x4> transformCache;

    for (auto physicsNode : std::as_const(m_physicsNodes))
        physicsNode->preSync(deltaTime, transformCache);

    m_jolt->Update(deltaTime, m_collisionSteps, m_tempAllocator, m_jobSystem);

    for (auto physicsNode : std::as_const(m_physicsNodes))
        physicsNode->sync();

    emit frameDone(deltaTime);

    m_updateAnimation->setDirty(false);
}

void PhysicsSystem::refresh()
{
    if (!m_running)
        m_animation->setCurrentTime(m_time);
}

void PhysicsSystem::emitContactCallbacks()
{
    if (m_contactListener == nullptr)
        return;

    for (const auto &bodyContact : std::as_const(m_contactListener->m_bodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyContact(qtBody2, bodyContact.manifold.baseOffset, bodyContact.manifold.relativeContactPointsOn1, bodyContact.manifold.worldSpaceNormal, bodyContact.manifold.penetrationDepth);
            emit qtBody2->bodyContact(qtBody1, bodyContact.manifold.baseOffset, bodyContact.manifold.relativeContactPointsOn2, -bodyContact.manifold.worldSpaceNormal, bodyContact.manifold.penetrationDepth);
        }
    }

    for (const auto &bodyContact : std::as_const(m_contactListener->m_enteredBodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyEntered(qtBody2);
            emit qtBody2->bodyEntered(qtBody1);
        }
    }

    for (const auto &bodyContact : std::as_const(m_contactListener->m_exitedBodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyExited(qtBody2);
            emit qtBody2->bodyExited(qtBody1);
        }
    }

    m_contactListener->m_bodyContacts.clear();
    m_contactListener->m_enteredBodyContacts.clear();
    m_contactListener->m_exitedBodyContacts.clear();
}
