#include "quick3dskeletonutils_p.h"
#include "physicsutils_p.h"

#include <QtQml/QQmlListReference>

#include <Jolt/Jolt.h>

namespace {

QMatrix4x4 localMatrix(const QQuick3DNode *node)
{
    QMatrix4x4 matrix;
    matrix.translate(node->position());
    matrix.rotate(node->rotation());
    matrix.scale(node->scale());
    return matrix;
}

void decomposeMatrix(const QMatrix4x4 &matrix, QVector3D &position, QQuaternion &rotation,
                     QVector3D &scale)
{
    position = matrix.column(3).toVector3D();

    QVector3D columns[3] = {
        matrix.column(0).toVector3D(),
        matrix.column(1).toVector3D(),
        matrix.column(2).toVector3D(),
    };

    scale = QVector3D(columns[0].length(), columns[1].length(), columns[2].length());

    const float det = QVector3D::dotProduct(columns[0],
                                            QVector3D::crossProduct(columns[1], columns[2]));
    if (det < 0.0f)
        scale.setX(-scale.x());

    QMatrix3x3 rotationMatrix;
    for (int col = 0; col < 3; ++col) {
        const float s = scale[col];
        const QVector3D column = qFuzzyIsNull(s) ? columns[col] : columns[col] / s;
        for (int row = 0; row < 3; ++row)
            rotationMatrix(row, col) = column[row];
    }
    rotation = QQuaternion::fromRotationMatrix(rotationMatrix);
}

int findParentJointIndex(QQuick3DNode *joint, const QHash<QQuick3DNode *, int> &jointToIndex)
{
    for (QQuick3DNode *parent = joint->parentNode(); parent != nullptr; parent = parent->parentNode()) {
        const auto it = jointToIndex.constFind(parent);
        if (it != jointToIndex.cend())
            return it.value();
    }
    return -1;
}

} // namespace

namespace Quick3DSkeletonUtils {

QString jointName(const QQuick3DNode *joint, int fallbackIndex)
{
    if (joint == nullptr)
        return QString();

    if (!joint->objectName().isEmpty())
        return joint->objectName();

    return QStringLiteral("Joint_%1").arg(fallbackIndex);
}

QVector<RenderJoint> collectRenderJoints(QQuick3DSkin *skin)
{
    QVector<RenderJoint> joints;
    if (skin == nullptr)
        return joints;

    QQmlListReference jointsRef(skin, "joints");
    if (!jointsRef.isValid())
        return joints;

    QHash<QQuick3DNode *, int> jointToIndex;
    joints.reserve(jointsRef.count());

    for (qsizetype i = 0; i < jointsRef.count(); ++i) {
        auto *joint = qobject_cast<QQuick3DNode *>(jointsRef.at(i));
        if (joint == nullptr)
            continue;

        RenderJoint entry;
        entry.joint = joint;
        entry.name = jointName(joint, joints.size());
        jointToIndex.insert(joint, joints.size());
        joints.append(entry);
    }

    for (RenderJoint &entry : joints)
        entry.parentIndex = findParentJointIndex(entry.joint, jointToIndex);

    return joints;
}

JPH::Ref<JPH::Skeleton> createJoltSkeleton(const QVector<RenderJoint> &joints)
{
    if (joints.isEmpty())
        return nullptr;

    auto skeleton = new JPH::Skeleton;
    for (const RenderJoint &joint : joints) {
        const QByteArray name = joint.name.toUtf8();
        if (joint.parentIndex < 0)
            skeleton->AddJoint(name.constData());
        else
            skeleton->AddJoint(name.constData(), joints.at(joint.parentIndex).name.toUtf8().constData());
    }

    skeleton->CalculateParentJointIndices();
    return skeleton;
}

JPH::SkeletonPose::Mat44Vector getModelSpacePose(QQuick3DNode *modelNode,
                                                     const QVector<RenderJoint> &joints)
{
    JPH::SkeletonPose::Mat44Vector pose;
    pose.resize(joints.size());
    if (joints.isEmpty())
        return pose;

    QMatrix4x4 toModelSpace;
    if (modelNode != nullptr)
        toModelSpace = modelNode->sceneTransform().inverted();

    for (int i = 0; i < joints.size(); ++i)
        pose[i] = PhysicsUtils::toJoltType(toModelSpace * joints.at(i).joint->sceneTransform());

    return pose;
}

JPH::SkeletonPose::Mat44Vector getLocalSpacePose(const QVector<RenderJoint> &joints)
{
    JPH::SkeletonPose::Mat44Vector pose;
    pose.resize(joints.size());

    for (int i = 0; i < joints.size(); ++i)
        pose[i] = PhysicsUtils::toJoltType(localMatrix(joints.at(i).joint));

    return pose;
}

JPH::SkeletonPose::Mat44Vector worldToModelSpace(QQuick3DNode *modelNode,
                                                 const JPH::SkeletonPose::Mat44Vector &worldPose)
{
    JPH::SkeletonPose::Mat44Vector modelPose = worldPose;
    if (modelNode == nullptr || worldPose.empty())
        return modelPose;

    const JPH::Mat44 worldToModel = PhysicsUtils::toJoltType(modelNode->sceneTransform().inverted());
    for (size_t i = 0; i < modelPose.size(); ++i)
        modelPose[i] = worldToModel * modelPose[i];

    return modelPose;
}

void applyModelSpacePose(const QVector<RenderJoint> &joints,
                         const JPH::SkeletonPose::Mat44Vector &modelSpacePose)
{
    if (joints.isEmpty() || modelSpacePose.size() != joints.size())
        return;

    for (int i = 0; i < joints.size(); ++i) {
        QQuick3DNode *joint = joints.at(i).joint;
        if (joint == nullptr)
            continue;

        const QMatrix4x4 model = PhysicsUtils::toQtType(modelSpacePose[i]);

        QMatrix4x4 local;
        const int parentIndex = joints.at(i).parentIndex;
        if (parentIndex >= 0)
            local = PhysicsUtils::toQtType(modelSpacePose[parentIndex]).inverted() * model;
        else
            local = model;

        QVector3D position;
        QQuaternion rotation;
        QVector3D scale;
        decomposeMatrix(local, position, rotation, scale);
        if (joint->position() != position)
            joint->setPosition(position);
        if (joint->rotation() != rotation)
            joint->setRotation(rotation);
        if (joint->scale() != scale)
            joint->setScale(scale);
    }
}

} // namespace Quick3DSkeletonUtils
