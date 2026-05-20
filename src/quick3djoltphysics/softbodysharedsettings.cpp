#include "softbodysharedsettings.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/SoftBody/SoftBodySharedSettings.h>

#include <utility>

// --- SoftBodyVertex ---

SoftBodyVertex::SoftBodyVertex(QObject *parent)
    : QObject(parent)
{
}

SoftBodyVertex::~SoftBodyVertex() = default;

QVector3D SoftBodyVertex::position() const
{
    return m_position;
}

void SoftBodyVertex::setPosition(const QVector3D &position)
{
    if (m_position == position)
        return;
    m_position = position;
    emit positionChanged(m_position);
}

QVector3D SoftBodyVertex::velocity() const
{
    return m_velocity;
}

void SoftBodyVertex::setVelocity(const QVector3D &velocity)
{
    if (m_velocity == velocity)
        return;
    m_velocity = velocity;
    emit velocityChanged(m_velocity);
}

float SoftBodyVertex::inverseMass() const
{
    return m_inverseMass;
}

void SoftBodyVertex::setInverseMass(float inverseMass)
{
    if (qFuzzyCompare(m_inverseMass, inverseMass))
        return;
    m_inverseMass = inverseMass;
    emit inverseMassChanged(m_inverseMass);
}

QVector2D SoftBodyVertex::textureCoordinate() const
{
    return m_textureCoordinate;
}

void SoftBodyVertex::setTextureCoordinate(const QVector2D &textureCoordinate)
{
    if (m_textureCoordinate == textureCoordinate)
        return;
    m_textureCoordinate = textureCoordinate;
    emit textureCoordinateChanged(m_textureCoordinate);
}

bool SoftBodyVertex::hasTextureCoordinate() const
{
    return m_hasTextureCoordinate;
}

void SoftBodyVertex::setHasTextureCoordinate(bool hasTextureCoordinate)
{
    if (m_hasTextureCoordinate == hasTextureCoordinate)
        return;
    m_hasTextureCoordinate = hasTextureCoordinate;
    emit hasTextureCoordinateChanged(m_hasTextureCoordinate);
}

// --- SoftBodyFace ---

SoftBodyFace::SoftBodyFace(QObject *parent)
    : QObject(parent)
{
}

SoftBodyFace::~SoftBodyFace() = default;

int SoftBodyFace::vertex1() const
{
    return m_vertex1;
}

void SoftBodyFace::setVertex1(int vertex1)
{
    if (m_vertex1 == vertex1)
        return;
    m_vertex1 = vertex1;
    emit vertex1Changed(m_vertex1);
}

int SoftBodyFace::vertex2() const
{
    return m_vertex2;
}

void SoftBodyFace::setVertex2(int vertex2)
{
    if (m_vertex2 == vertex2)
        return;
    m_vertex2 = vertex2;
    emit vertex2Changed(m_vertex2);
}

int SoftBodyFace::vertex3() const
{
    return m_vertex3;
}

void SoftBodyFace::setVertex3(int vertex3)
{
    if (m_vertex3 == vertex3)
        return;
    m_vertex3 = vertex3;
    emit vertex3Changed(m_vertex3);
}

int SoftBodyFace::materialIndex() const
{
    return m_materialIndex;
}

void SoftBodyFace::setMaterialIndex(int materialIndex)
{
    if (m_materialIndex == materialIndex)
        return;
    m_materialIndex = materialIndex;
    emit materialIndexChanged(m_materialIndex);
}

// --- SoftBodyEdge ---

SoftBodyEdge::SoftBodyEdge(QObject *parent)
    : QObject(parent)
{
}

SoftBodyEdge::~SoftBodyEdge() = default;

int SoftBodyEdge::vertex1() const
{
    return m_vertex1;
}

void SoftBodyEdge::setVertex1(int vertex1)
{
    if (m_vertex1 == vertex1)
        return;
    m_vertex1 = vertex1;
    emit vertex1Changed(m_vertex1);
}

int SoftBodyEdge::vertex2() const
{
    return m_vertex2;
}

void SoftBodyEdge::setVertex2(int vertex2)
{
    if (m_vertex2 == vertex2)
        return;
    m_vertex2 = vertex2;
    emit vertex2Changed(m_vertex2);
}

float SoftBodyEdge::compliance() const
{
    return m_compliance;
}

void SoftBodyEdge::setCompliance(float compliance)
{
    if (qFuzzyCompare(m_compliance, compliance))
        return;
    m_compliance = compliance;
    emit complianceChanged(m_compliance);
}

// --- SoftBodyVolume ---

SoftBodyVolume::SoftBodyVolume(QObject *parent)
    : QObject(parent)
{
}

SoftBodyVolume::~SoftBodyVolume() = default;

int SoftBodyVolume::vertex1() const
{
    return m_vertex1;
}

void SoftBodyVolume::setVertex1(int vertex1)
{
    if (m_vertex1 == vertex1)
        return;
    m_vertex1 = vertex1;
    emit vertex1Changed(m_vertex1);
}

int SoftBodyVolume::vertex2() const
{
    return m_vertex2;
}

void SoftBodyVolume::setVertex2(int vertex2)
{
    if (m_vertex2 == vertex2)
        return;
    m_vertex2 = vertex2;
    emit vertex2Changed(m_vertex2);
}

int SoftBodyVolume::vertex3() const
{
    return m_vertex3;
}

void SoftBodyVolume::setVertex3(int vertex3)
{
    if (m_vertex3 == vertex3)
        return;
    m_vertex3 = vertex3;
    emit vertex3Changed(m_vertex3);
}

int SoftBodyVolume::vertex4() const
{
    return m_vertex4;
}

void SoftBodyVolume::setVertex4(int vertex4)
{
    if (m_vertex4 == vertex4)
        return;
    m_vertex4 = vertex4;
    emit vertex4Changed(m_vertex4);
}

float SoftBodyVolume::compliance() const
{
    return m_compliance;
}

void SoftBodyVolume::setCompliance(float compliance)
{
    if (qFuzzyCompare(m_compliance, compliance))
        return;
    m_compliance = compliance;
    emit complianceChanged(m_compliance);
}

// --- SoftBodySharedSettings ---

SoftBodySharedSettings::SoftBodySharedSettings(QObject *parent)
    : QObject(parent)
{
}

SoftBodySharedSettings::~SoftBodySharedSettings()
{
    clear();
}

QQmlListProperty<SoftBodyVertex> SoftBodySharedSettings::vertices()
{
    return QQmlListProperty<SoftBodyVertex>(this, nullptr,
                                            &SoftBodySharedSettings::appendVertex,
                                            &SoftBodySharedSettings::vertexCount,
                                            &SoftBodySharedSettings::vertexAt,
                                            &SoftBodySharedSettings::clearVertices);
}

QQmlListProperty<SoftBodyFace> SoftBodySharedSettings::faces()
{
    return QQmlListProperty<SoftBodyFace>(this, nullptr,
                                          &SoftBodySharedSettings::appendFace,
                                          &SoftBodySharedSettings::faceCount,
                                          &SoftBodySharedSettings::faceAt,
                                          &SoftBodySharedSettings::clearFaces);
}

QQmlListProperty<SoftBodyEdge> SoftBodySharedSettings::edges()
{
    return QQmlListProperty<SoftBodyEdge>(this, nullptr,
                                          &SoftBodySharedSettings::appendEdge,
                                          &SoftBodySharedSettings::edgeCount,
                                          &SoftBodySharedSettings::edgeAt,
                                          &SoftBodySharedSettings::clearEdges);
}

QQmlListProperty<SoftBodyVolume> SoftBodySharedSettings::volumes()
{
    return QQmlListProperty<SoftBodyVolume>(this, nullptr,
                                            &SoftBodySharedSettings::appendVolume,
                                            &SoftBodySharedSettings::volumeCount,
                                            &SoftBodySharedSettings::volumeAt,
                                            &SoftBodySharedSettings::clearVolumes);
}

bool SoftBodySharedSettings::autoCreateConstraints() const
{
    return m_autoCreateConstraints;
}

void SoftBodySharedSettings::setAutoCreateConstraints(bool autoCreate)
{
    if (m_autoCreateConstraints == autoCreate)
        return;
    m_autoCreateConstraints = autoCreate;
    emit autoCreateConstraintsChanged(m_autoCreateConstraints);
}

float SoftBodySharedSettings::edgeCompliance() const
{
    return m_edgeCompliance;
}

void SoftBodySharedSettings::setEdgeCompliance(float compliance)
{
    if (qFuzzyCompare(m_edgeCompliance, compliance))
        return;
    m_edgeCompliance = compliance;
    emit edgeComplianceChanged(m_edgeCompliance);
}

float SoftBodySharedSettings::bendCompliance() const
{
    return m_bendCompliance;
}

void SoftBodySharedSettings::setBendCompliance(float compliance)
{
    if (qFuzzyCompare(m_bendCompliance, compliance))
        return;
    m_bendCompliance = compliance;
    emit bendComplianceChanged(m_bendCompliance);
}

bool SoftBodySharedSettings::usesRenderingTextureCoordinates() const
{
    return false;
}

void SoftBodySharedSettings::cacheRestPositionsForRendering(const QVector<QVector3D> &restPositions)
{
    Q_UNUSED(restPositions)
}

QVector2D SoftBodySharedSettings::textureCoordinateForRendering(int vertexIndex,
                                                              const QVector3D &restPosition,
                                                              const QVector3D &restFaceNormal) const
{
    Q_UNUSED(restPosition)
    Q_UNUSED(restFaceNormal)
    return textureCoordinateAt(vertexIndex);
}

void SoftBodySharedSettings::clearTopology()
{
    qDeleteAll(m_vertices);
    qDeleteAll(m_faces);
    qDeleteAll(m_edges);
    qDeleteAll(m_volumes);
    m_vertices.clear();
    m_faces.clear();
    m_edges.clear();
    m_volumes.clear();
}

void SoftBodySharedSettings::clear()
{
    clearTopology();
    emit topologyChanged();
}

void SoftBodySharedSettings::markTopologyChanged()
{
    emit topologyChanged();
}

SoftBodyVertex *SoftBodySharedSettings::topologyVertexAt(int index)
{
    if (index < 0 || index >= m_vertices.size())
        return nullptr;
    return m_vertices.at(index);
}

void SoftBodySharedSettings::addVertex(const QVector3D &position, float inverseMass)
{
    auto *v = new SoftBodyVertex(this);
    v->setPosition(position);
    v->setInverseMass(inverseMass);
    m_vertices.append(v);
}

void SoftBodySharedSettings::addVertex(const QVector3D &position, float inverseMass, const QVector2D &textureCoordinate)
{
    auto *v = new SoftBodyVertex(this);
    v->setPosition(position);
    v->setInverseMass(inverseMass);
    v->setTextureCoordinate(textureCoordinate);
    v->setHasTextureCoordinate(true);
    m_vertices.append(v);
}

bool SoftBodySharedSettings::hasExplicitTextureCoordinates() const
{
    if (m_vertices.isEmpty())
        return false;

    for (const SoftBodyVertex *vertex : m_vertices) {
        if (!vertex->hasTextureCoordinate())
            return false;
    }
    return true;
}

int SoftBodySharedSettings::topologyVertexCount() const
{
    return m_vertices.size();
}

QVector2D SoftBodySharedSettings::textureCoordinateAt(int index) const
{
    if (index < 0 || index >= m_vertices.size())
        return {};
    return m_vertices.at(index)->textureCoordinate();
}

void SoftBodySharedSettings::addFace(int vertex1, int vertex2, int vertex3, int materialIndex)
{
    auto *f = new SoftBodyFace(this);
    f->setVertex1(vertex1);
    f->setVertex2(vertex2);
    f->setVertex3(vertex3);
    f->setMaterialIndex(materialIndex);
    m_faces.append(f);
}

void SoftBodySharedSettings::addEdge(int vertex1, int vertex2, float compliance)
{
    auto *e = new SoftBodyEdge(this);
    e->setVertex1(vertex1);
    e->setVertex2(vertex2);
    e->setCompliance(compliance);
    m_edges.append(e);
}

void SoftBodySharedSettings::addVolume(int vertex1, int vertex2, int vertex3, int vertex4, float compliance)
{
    auto *v = new SoftBodyVolume(this);
    v->setVertex1(vertex1);
    v->setVertex2(vertex2);
    v->setVertex3(vertex3);
    v->setVertex4(vertex4);
    v->setCompliance(compliance);
    m_volumes.append(v);
}

JPH::Ref<JPH::SoftBodySharedSettings> SoftBodySharedSettings::createJoltSettings() const
{
    if (m_vertices.isEmpty()) {
        qWarning() << "SoftBodySharedSettings: no vertices defined";
        return nullptr;
    }

    auto settings = new JPH::SoftBodySharedSettings;

    settings->mVertices.reserve(m_vertices.size());
    for (const SoftBodyVertex *v : std::as_const(m_vertices)) {
        JPH::SoftBodySharedSettings::Vertex joltVertex;
        const JPH::Vec3 position = PhysicsUtils::toJoltType(v->position());
        const JPH::Vec3 velocity = PhysicsUtils::toJoltType(v->velocity());
        joltVertex.mPosition = JPH::Float3(position.GetX(), position.GetY(), position.GetZ());
        joltVertex.mVelocity = JPH::Float3(velocity.GetX(), velocity.GetY(), velocity.GetZ());
        joltVertex.mInvMass = v->inverseMass();
        settings->mVertices.push_back(joltVertex);
    }

    for (const SoftBodyFace *f : std::as_const(m_faces)) {
        settings->AddFace(JPH::SoftBodySharedSettings::Face(
            static_cast<uint32_t>(f->vertex1()),
            static_cast<uint32_t>(f->vertex2()),
            static_cast<uint32_t>(f->vertex3()),
            static_cast<uint32_t>(f->materialIndex())));
    }

    const bool hasExplicitConstraints = !m_edges.isEmpty() || !m_volumes.isEmpty();

    if (hasExplicitConstraints) {
        settings->mEdgeConstraints.reserve(m_edges.size());
        for (const SoftBodyEdge *e : std::as_const(m_edges)) {
            JPH::SoftBodySharedSettings::Edge joltEdge;
            joltEdge.mVertex[0] = static_cast<uint32_t>(e->vertex1());
            joltEdge.mVertex[1] = static_cast<uint32_t>(e->vertex2());
            joltEdge.mCompliance = e->compliance();
            settings->mEdgeConstraints.push_back(joltEdge);
        }
        if (!settings->mEdgeConstraints.empty())
            settings->CalculateEdgeLengths();

        settings->mVolumeConstraints.reserve(m_volumes.size());
        for (const SoftBodyVolume *vol : std::as_const(m_volumes)) {
            JPH::SoftBodySharedSettings::Volume joltVolume;
            joltVolume.mVertex[0] = static_cast<uint32_t>(vol->vertex1());
            joltVolume.mVertex[1] = static_cast<uint32_t>(vol->vertex2());
            joltVolume.mVertex[2] = static_cast<uint32_t>(vol->vertex3());
            joltVolume.mVertex[3] = static_cast<uint32_t>(vol->vertex4());
            joltVolume.mCompliance = vol->compliance();
            settings->mVolumeConstraints.push_back(joltVolume);
        }
        if (!settings->mVolumeConstraints.empty())
            settings->CalculateVolumeConstraintVolumes();
    } else if (m_autoCreateConstraints && !m_faces.isEmpty()) {
        JPH::SoftBodySharedSettings::VertexAttributes va(m_edgeCompliance, m_edgeCompliance, m_bendCompliance);
        settings->CreateConstraints(&va, 1);
    }

    settings->Optimize();

    return settings;
}

// --- List property helpers ---

void SoftBodySharedSettings::appendVertex(QQmlListProperty<SoftBodyVertex> *list, SoftBodyVertex *vertex)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_vertices.append(vertex);
}

qsizetype SoftBodySharedSettings::vertexCount(QQmlListProperty<SoftBodyVertex> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_vertices.size();
}

SoftBodyVertex *SoftBodySharedSettings::vertexAt(QQmlListProperty<SoftBodyVertex> *list, qsizetype index)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_vertices.at(index);
}

void SoftBodySharedSettings::clearVertices(QQmlListProperty<SoftBodyVertex> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_vertices.clear();
}

void SoftBodySharedSettings::appendFace(QQmlListProperty<SoftBodyFace> *list, SoftBodyFace *face)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_faces.append(face);
}

qsizetype SoftBodySharedSettings::faceCount(QQmlListProperty<SoftBodyFace> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_faces.size();
}

SoftBodyFace *SoftBodySharedSettings::faceAt(QQmlListProperty<SoftBodyFace> *list, qsizetype index)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_faces.at(index);
}

void SoftBodySharedSettings::clearFaces(QQmlListProperty<SoftBodyFace> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_faces.clear();
}

void SoftBodySharedSettings::appendEdge(QQmlListProperty<SoftBodyEdge> *list, SoftBodyEdge *edge)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_edges.append(edge);
}

qsizetype SoftBodySharedSettings::edgeCount(QQmlListProperty<SoftBodyEdge> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_edges.size();
}

SoftBodyEdge *SoftBodySharedSettings::edgeAt(QQmlListProperty<SoftBodyEdge> *list, qsizetype index)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_edges.at(index);
}

void SoftBodySharedSettings::clearEdges(QQmlListProperty<SoftBodyEdge> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_edges.clear();
}

void SoftBodySharedSettings::appendVolume(QQmlListProperty<SoftBodyVolume> *list, SoftBodyVolume *volume)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_volumes.append(volume);
}

qsizetype SoftBodySharedSettings::volumeCount(QQmlListProperty<SoftBodyVolume> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_volumes.size();
}

SoftBodyVolume *SoftBodySharedSettings::volumeAt(QQmlListProperty<SoftBodyVolume> *list, qsizetype index)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    return self->m_volumes.at(index);
}

void SoftBodySharedSettings::clearVolumes(QQmlListProperty<SoftBodyVolume> *list)
{
    auto *self = static_cast<SoftBodySharedSettings *>(list->object);
    self->m_volumes.clear();
}
