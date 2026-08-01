#ifndef JOLTDEBUGRENDERER_P_H
#define JOLTDEBUGRENDERER_P_H

#include "debuggeometry_p.h"
#include "physicsutils_p.h"

#include <Jolt/Renderer/DebugRendererSimple.h>

class JoltDebugRenderer final : public JPH::DebugRendererSimple
{
public:
    void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override
    {
        const quint32 c = inColor.GetUInt32();
        m_vertices.push_back({ PhysicsUtils::toQtType(JPH::Vec3(inFrom)), c });
        m_vertices.push_back({ PhysicsUtils::toQtType(JPH::Vec3(inTo)), c });
    }

    void DrawText3D(JPH::RVec3Arg inPosition,
                    const JPH::string_view &inString,
                    JPH::ColorArg inColor,
                    float inHeight) override
    {
        Q_UNUSED(inPosition)
        Q_UNUSED(inString)
        Q_UNUSED(inColor)
        Q_UNUSED(inHeight)
    }

    void clear() { m_vertices.clear(); }

    const QVector<DebugLineVertex> &vertices() const { return m_vertices; }

private:
    QVector<DebugLineVertex> m_vertices;
};

#endif // JOLTDEBUGRENDERER_P_H
