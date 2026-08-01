#include "ragdollpart_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>

#include <QtQuick3D/private/qquick3dobject_p.h>

RagdollPart::RagdollPart(QQuick3DNode *parent)
    : QQuick3DNode(parent)
{
}

RagdollPart::~RagdollPart() = default;

QString RagdollPart::jointName() const
{
    return m_jointName;
}

void RagdollPart::setJointName(const QString &jointName)
{
    if (m_jointName == jointName)
        return;
    m_jointName = jointName;
    emit jointNameChanged(m_jointName);
}

QString RagdollPart::resolvedJointName() const
{
    if (!m_jointName.isEmpty())
        return m_jointName;
    return objectName();
}

AbstractShape *RagdollPart::shape() const
{
    return m_shape;
}

void RagdollPart::setShape(AbstractShape *shape)
{
    if (m_shape == shape)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &RagdollPart::setShape, shape, m_shape);

    if (m_shape != nullptr)
        QObject::disconnect(m_shapeSignalConnection);

    m_shape = shape;

    if (m_shape != nullptr) {
        if (m_shape->parentItem() == nullptr)
            m_shape->setParentItem(this);

        m_shapeSignalConnection = connect(m_shape, &AbstractShape::changed, this, [this] {
            emit shapeChanged(m_shape);
        });
        connect(m_shape, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_shape == obj)
                setShape(nullptr);
        });
    }

    emit shapeChanged(m_shape);
}

float RagdollPart::mass() const
{
    return m_mass;
}

void RagdollPart::setMass(float mass)
{
    if (qFuzzyCompare(m_mass, mass))
        return;
    m_mass = mass;
    emit massChanged(m_mass);
}

RagdollPart::MotionType RagdollPart::motionType() const
{
    return m_motionType;
}

void RagdollPart::setMotionType(MotionType motionType)
{
    if (m_motionType == motionType)
        return;
    m_motionType = motionType;
    emit motionTypeChanged(m_motionType);
}

RagdollPart::MotionQuality RagdollPart::motionQuality() const
{
    return m_motionQuality;
}

void RagdollPart::setMotionQuality(MotionQuality motionQuality)
{
    if (m_motionQuality == motionQuality)
        return;
    m_motionQuality = motionQuality;
    emit motionQualityChanged(m_motionQuality);
}

int RagdollPart::objectLayer() const
{
    return m_objectLayer;
}

void RagdollPart::setObjectLayer(int objectLayer)
{
    if (m_objectLayer == objectLayer)
        return;
    m_objectLayer = objectLayer;
    emit objectLayerChanged(m_objectLayer);
}

AbstractTwoBodyPhysicsConstraintSettings *RagdollPart::constraintToParent() const
{
    return m_constraintToParent;
}

void RagdollPart::setConstraintToParent(AbstractTwoBodyPhysicsConstraintSettings *constraintToParent)
{
    if (m_constraintToParent == constraintToParent)
        return;

    if (m_constraintToParent != nullptr)
        QObject::disconnect(m_constraintSignalConnection);

    m_constraintToParent = constraintToParent;

    if (m_constraintToParent != nullptr) {
        if (m_constraintToParent->parent() == nullptr)
            m_constraintToParent->setParent(this);

        m_constraintSignalConnection = connect(m_constraintToParent,
                                               &AbstractPhysicsConstraintSettings::changed, this,
                                               [this] {
                                                   emit constraintToParentChanged(m_constraintToParent);
                                               });
        connect(m_constraintToParent, &QObject::destroyed, this, [this](QObject *obj) {
            if (m_constraintToParent == obj)
                setConstraintToParent(nullptr);
        });
    }

    emit constraintToParentChanged(m_constraintToParent);
}

Body *RagdollPart::body() const
{
    return m_body;
}

void RagdollPart::setBody(Body *body)
{
    if (m_body == body)
        return;

    m_body = body;
    emit bodyChanged(m_body);
}

void RagdollPart::syncFromScene(const QVector3D &scenePosition, const QQuaternion &sceneRotation)
{
    auto *parentNode = qobject_cast<QQuick3DNode *>(parentItem());
    if (parentNode == nullptr) {
        setPosition(scenePosition);
        setRotation(sceneRotation);
        return;
    }

    setPosition(parentNode->mapPositionFromScene(scenePosition));
    setRotation(parentNode->sceneRotation().inverted() * sceneRotation);
}

JPH::EMotionType RagdollPart::resolvedMotionType(Body::MotionType ragdollDefault) const
{
    switch (m_motionType) {
    case MotionType::Kinematic:
        return JPH::EMotionType::Kinematic;
    case MotionType::Dynamic:
        return JPH::EMotionType::Dynamic;
    case MotionType::Inherit:
        break;
    }
    return static_cast<JPH::EMotionType>(ragdollDefault);
}

JPH::EMotionQuality RagdollPart::resolvedMotionQuality(Body::MotionQuality ragdollDefault) const
{
    switch (m_motionQuality) {
    case MotionQuality::Discrete:
        return JPH::EMotionQuality::Discrete;
    case MotionQuality::LinearCast:
        return JPH::EMotionQuality::LinearCast;
    case MotionQuality::Inherit:
        break;
    }
    return static_cast<JPH::EMotionQuality>(ragdollDefault);
}

JPH::ObjectLayer RagdollPart::resolvedObjectLayer(int ragdollDefault) const
{
    if (m_objectLayer >= 0)
        return static_cast<JPH::ObjectLayer>(m_objectLayer);
    return static_cast<JPH::ObjectLayer>(ragdollDefault);
}

JPH::RagdollSettings::Part RagdollPart::createJoltPart(Body::MotionType defaultMotionType,
                                                       Body::MotionQuality defaultMotionQuality,
                                                       int defaultObjectLayer) const
{
    JPH::RagdollSettings::Part part;

    if (m_shape == nullptr) {
        qWarning() << "RagdollPart" << resolvedJointName() << ": no shape defined";
        return part;
    }

    const JPH::Ref<JPH::Shape> shape = m_shape->getJoltShape();
    if (!shape) {
        qWarning() << "RagdollPart" << resolvedJointName() << ": failed to create shape";
        return part;
    }

    part.SetShape(shape);
    part.mPosition = PhysicsUtils::toJoltType(scenePosition());
    part.mRotation = PhysicsUtils::toJoltType(sceneRotation());
    part.mMotionType = resolvedMotionType(defaultMotionType);
    part.mMotionQuality = resolvedMotionQuality(defaultMotionQuality);
    part.mObjectLayer = resolvedObjectLayer(defaultObjectLayer);

    if (m_mass > 0.0f) {
        part.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
        part.mMassPropertiesOverride.mMass = m_mass;
    }

    if (m_constraintToParent != nullptr) {
        auto *parentPart = qobject_cast<RagdollPart *>(parentItem());
        part.mToParent = m_constraintToParent->createJoltTwoBodyConstraintSettings(parentPart);
    }

    return part;
}
