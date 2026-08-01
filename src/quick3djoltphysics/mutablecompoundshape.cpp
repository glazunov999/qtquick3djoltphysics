#include "mutablecompoundshape_p.h"
#include "body_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/DecoratedShape.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>

MutableCompoundShape::MutableCompoundShape(QQuick3DNode *parent) : AbstractCompoundShape(parent)
{
}

MutableCompoundShape::~MutableCompoundShape() = default;

static JPH::MutableCompoundShape *getInnerMutableCompoundShape(JPH::Shape *shape)
{
    while (shape != nullptr && shape->GetType() == JPH::EShapeType::Decorated) {
        auto *decorated = static_cast<JPH::DecoratedShape *>(shape);
        shape = const_cast<JPH::Shape *>(decorated->GetInnerShape());
    }

    if (shape != nullptr && shape->GetSubType() == JPH::EShapeSubType::MutableCompound)
        return static_cast<JPH::MutableCompoundShape *>(shape);

    return nullptr;
}

void MutableCompoundShape::addShape(AbstractShape *shape)
{
    if (shape == nullptr)
        return;

    auto *mutableShape = getInnerMutableCompoundShape(m_joltShape.GetPtr());
    appendShape(shape);

    if (mutableShape == nullptr) {
        handleShapeChange();
        return;
    }

    const QVector3D previousCenterOfMass = PhysicsUtils::toQtType(mutableShape->GetCenterOfMass());
    mutableShape->AddShape(PhysicsUtils::toJoltType(shape->position()),
                           PhysicsUtils::toJoltType(shape->rotation()),
                           shape->getJoltShape());
    mutableShape->AdjustCenterOfMass();
    notifyBodyShapeChanged(previousCenterOfMass, true);
}

void MutableCompoundShape::removeShape(int index)
{
    if (index < 0 || index >= m_shapes.size())
        return;

    auto *mutableShape = getInnerMutableCompoundShape(m_joltShape.GetPtr());
    if (mutableShape == nullptr || uint(m_shapes.size()) != mutableShape->GetNumSubShapes()) {
        AbstractShape *shape = m_shapes.at(index);
        shape->disconnect(this);
        m_shapes.removeAt(index);
        handleShapeChange();
        return;
    }

    AbstractShape *shape = m_shapes.at(index);
    shape->disconnect(this);
    m_shapes.removeAt(index);

    const QVector3D previousCenterOfMass = PhysicsUtils::toQtType(mutableShape->GetCenterOfMass());
    mutableShape->RemoveShape(uint(index));
    mutableShape->AdjustCenterOfMass();
    notifyBodyShapeChanged(previousCenterOfMass, true);
}

void MutableCompoundShape::modifyShapes(bool updateMassProperties)
{
    auto *mutableShape = getInnerMutableCompoundShape(m_joltShape.GetPtr());
    if (mutableShape == nullptr || uint(m_shapes.size()) != mutableShape->GetNumSubShapes()) {
        handleShapeChange();
        return;
    }

    struct PositionRotation
    {
        JPH::Vec3 mPosition;
        JPH::Quat mRotation;
    };

    QVector<PositionRotation> transforms(m_shapes.size());
    for (int i = 0; i < m_shapes.size(); ++i) {
        transforms[i].mPosition = PhysicsUtils::toJoltType(m_shapes.at(i)->position());
        transforms[i].mRotation = PhysicsUtils::toJoltType(m_shapes.at(i)->rotation());
    }

    const QVector3D previousCenterOfMass = PhysicsUtils::toQtType(mutableShape->GetCenterOfMass());
    mutableShape->ModifyShapes(0,
                               uint(transforms.size()),
                               &transforms.front().mPosition,
                               &transforms.front().mRotation,
                               sizeof(PositionRotation),
                               sizeof(PositionRotation));
    mutableShape->AdjustCenterOfMass();
    notifyBodyShapeChanged(previousCenterOfMass, updateMassProperties);
}

void MutableCompoundShape::createJoltShape()
{
    if (!isComponentComplete() || m_shapes.isEmpty())
        return;

    JPH::MutableCompoundShapeSettings mutableCompoundSettings;
    for (auto *shape : std::as_const(m_shapes)) {
        mutableCompoundSettings.AddShape(PhysicsUtils::toJoltType(shape->position()),
                                         PhysicsUtils::toJoltType(shape->rotation()),
                                         shape->getJoltShape());
    }

    const JPH::ShapeSettings::ShapeResult result = mutableCompoundSettings.Create();
    if (result.HasError()) {
        qWarning() << "MutableCompoundShape: failed to create shape:" << result.GetError().c_str();
        return;
    }

    m_joltShape = new JPH::ScaledShape(result.Get(), PhysicsUtils::toJoltType(physicsScale()));
}

void MutableCompoundShape::notifyBodyShapeChanged(const QVector3D &previousCenterOfMass, bool updateMassProperties)
{
    for (QQuick3DNode *node = parentNode(); node != nullptr; node = node->parentNode()) {
        if (auto *body = qobject_cast<Body *>(node)) {
            body->notifyShapeChanged(previousCenterOfMass, updateMassProperties);
            return;
        }
    }
}
