#ifndef QUICK3DSKELETONUTILS_P_H
#define QUICK3DSKELETONUTILS_P_H

#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQuick3D/private/qquick3dskin_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Skeleton/Skeleton.h>
#include <Jolt/Skeleton/SkeletonPose.h>

#include <QVector>

namespace Quick3DSkeletonUtils {

struct RenderJoint
{
    QQuick3DNode *joint = nullptr;
    QString name;
    int parentIndex = -1;
};

JPH::Ref<JPH::Skeleton> createJoltSkeleton(const QVector<RenderJoint> &joints);
QVector<RenderJoint> collectRenderJoints(QQuick3DSkin *skin);

JPH::SkeletonPose::Mat44Vector getModelSpacePose(QQuick3DNode *modelNode,
                                                     const QVector<RenderJoint> &joints);
JPH::SkeletonPose::Mat44Vector getLocalSpacePose(const QVector<RenderJoint> &joints);

JPH::SkeletonPose::Mat44Vector worldToModelSpace(QQuick3DNode *modelNode,
                                                 const JPH::SkeletonPose::Mat44Vector &worldPose);

void applyModelSpacePose(const QVector<RenderJoint> &joints,
                         const JPH::SkeletonPose::Mat44Vector &modelSpacePose);

QString jointName(const QQuick3DNode *joint, int fallbackIndex);

} // namespace Quick3DSkeletonUtils

#endif // QUICK3DSKELETONUTILS_P_H
