#ifndef SOFTBODYSHAREDSETTINGS_H
#define SOFTBODYSHAREDSETTINGS_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>
#include <QObject>
#include <QVector2D>
#include <QVector3D>

#include <cfloat>

namespace JPH {
template <class T> class Ref;
class SoftBodySharedSettings;
}

class SoftBody;

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodyVertex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)
    Q_PROPERTY(float inverseMass READ inverseMass WRITE setInverseMass NOTIFY inverseMassChanged)
    Q_PROPERTY(QVector2D textureCoordinate READ textureCoordinate WRITE setTextureCoordinate NOTIFY textureCoordinateChanged)
    Q_PROPERTY(bool hasTextureCoordinate READ hasTextureCoordinate WRITE setHasTextureCoordinate NOTIFY hasTextureCoordinateChanged)
    QML_NAMED_ELEMENT(SoftBodyVertex)
public:
    explicit SoftBodyVertex(QObject *parent = nullptr);
    ~SoftBodyVertex() override;

    QVector3D position() const;
    void setPosition(const QVector3D &position);

    QVector3D velocity() const;
    void setVelocity(const QVector3D &velocity);

    float inverseMass() const;
    void setInverseMass(float inverseMass);

    QVector2D textureCoordinate() const;
    void setTextureCoordinate(const QVector2D &textureCoordinate);

    bool hasTextureCoordinate() const;
    void setHasTextureCoordinate(bool hasTextureCoordinate);

signals:
    void positionChanged(const QVector3D &position);
    void velocityChanged(const QVector3D &velocity);
    void inverseMassChanged(float inverseMass);
    void textureCoordinateChanged(const QVector2D &textureCoordinate);
    void hasTextureCoordinateChanged(bool hasTextureCoordinate);

private:
    QVector3D m_position;
    QVector3D m_velocity;
    float m_inverseMass = 1.0f;
    QVector2D m_textureCoordinate;
    bool m_hasTextureCoordinate = false;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodyFace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
    Q_PROPERTY(int vertex2 READ vertex2 WRITE setVertex2 NOTIFY vertex2Changed)
    Q_PROPERTY(int vertex3 READ vertex3 WRITE setVertex3 NOTIFY vertex3Changed)
    Q_PROPERTY(int materialIndex READ materialIndex WRITE setMaterialIndex NOTIFY materialIndexChanged)
    QML_NAMED_ELEMENT(SoftBodyFace)
public:
    explicit SoftBodyFace(QObject *parent = nullptr);
    ~SoftBodyFace() override;

    int vertex1() const;
    void setVertex1(int vertex1);

    int vertex2() const;
    void setVertex2(int vertex2);

    int vertex3() const;
    void setVertex3(int vertex3);

    int materialIndex() const;
    void setMaterialIndex(int materialIndex);

signals:
    void vertex1Changed(int vertex1);
    void vertex2Changed(int vertex2);
    void vertex3Changed(int vertex3);
    void materialIndexChanged(int materialIndex);

private:
    int m_vertex1 = 0;
    int m_vertex2 = 0;
    int m_vertex3 = 0;
    int m_materialIndex = 0;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodyEdge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
    Q_PROPERTY(int vertex2 READ vertex2 WRITE setVertex2 NOTIFY vertex2Changed)
    Q_PROPERTY(float compliance READ compliance WRITE setCompliance NOTIFY complianceChanged)
    QML_NAMED_ELEMENT(SoftBodyEdge)
public:
    explicit SoftBodyEdge(QObject *parent = nullptr);
    ~SoftBodyEdge() override;

    int vertex1() const;
    void setVertex1(int vertex1);

    int vertex2() const;
    void setVertex2(int vertex2);

    float compliance() const;
    void setCompliance(float compliance);

signals:
    void vertex1Changed(int vertex1);
    void vertex2Changed(int vertex2);
    void complianceChanged(float compliance);

private:
    int m_vertex1 = 0;
    int m_vertex2 = 0;
    float m_compliance = 0.0f;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodyVolume : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
    Q_PROPERTY(int vertex2 READ vertex2 WRITE setVertex2 NOTIFY vertex2Changed)
    Q_PROPERTY(int vertex3 READ vertex3 WRITE setVertex3 NOTIFY vertex3Changed)
    Q_PROPERTY(int vertex4 READ vertex4 WRITE setVertex4 NOTIFY vertex4Changed)
    Q_PROPERTY(float compliance READ compliance WRITE setCompliance NOTIFY complianceChanged)
    QML_NAMED_ELEMENT(SoftBodyVolume)
public:
    explicit SoftBodyVolume(QObject *parent = nullptr);
    ~SoftBodyVolume() override;

    int vertex1() const;
    void setVertex1(int vertex1);

    int vertex2() const;
    void setVertex2(int vertex2);

    int vertex3() const;
    void setVertex3(int vertex3);

    int vertex4() const;
    void setVertex4(int vertex4);

    float compliance() const;
    void setCompliance(float compliance);

signals:
    void vertex1Changed(int vertex1);
    void vertex2Changed(int vertex2);
    void vertex3Changed(int vertex3);
    void vertex4Changed(int vertex4);
    void complianceChanged(float compliance);

private:
    int m_vertex1 = 0;
    int m_vertex2 = 0;
    int m_vertex3 = 0;
    int m_vertex4 = 0;
    float m_compliance = 0.0f;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT SoftBodySharedSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SoftBodyVertex> vertices READ vertices CONSTANT)
    Q_PROPERTY(QQmlListProperty<SoftBodyFace> faces READ faces CONSTANT)
    Q_PROPERTY(QQmlListProperty<SoftBodyEdge> edges READ edges CONSTANT)
    Q_PROPERTY(QQmlListProperty<SoftBodyVolume> volumes READ volumes CONSTANT)
    Q_PROPERTY(bool autoCreateConstraints READ autoCreateConstraints WRITE setAutoCreateConstraints NOTIFY autoCreateConstraintsChanged)
    Q_PROPERTY(float edgeCompliance READ edgeCompliance WRITE setEdgeCompliance NOTIFY edgeComplianceChanged)
    Q_PROPERTY(float bendCompliance READ bendCompliance WRITE setBendCompliance NOTIFY bendComplianceChanged)
    QML_NAMED_ELEMENT(SoftBodySharedSettings)
public:
    explicit SoftBodySharedSettings(QObject *parent = nullptr);
    ~SoftBodySharedSettings() override;

    QQmlListProperty<SoftBodyVertex> vertices();
    QQmlListProperty<SoftBodyFace> faces();
    QQmlListProperty<SoftBodyEdge> edges();
    QQmlListProperty<SoftBodyVolume> volumes();

    bool autoCreateConstraints() const;
    void setAutoCreateConstraints(bool autoCreate);

    float edgeCompliance() const;
    void setEdgeCompliance(float compliance);

    float bendCompliance() const;
    void setBendCompliance(float compliance);

    Q_INVOKABLE void clear();
    Q_INVOKABLE void addVertex(const QVector3D &position, float inverseMass = 1.0f);
    Q_INVOKABLE void addVertex(const QVector3D &position, float inverseMass, const QVector2D &textureCoordinate);

    bool hasExplicitTextureCoordinates() const;
    int topologyVertexCount() const;
    QVector2D textureCoordinateAt(int index) const;
    Q_INVOKABLE void addFace(int vertex1, int vertex2, int vertex3, int materialIndex = 0);
    Q_INVOKABLE void addEdge(int vertex1, int vertex2, float compliance = 0.0f);
    Q_INVOKABLE void addVolume(int vertex1, int vertex2, int vertex3, int vertex4, float compliance = 0.0f);

signals:
    void autoCreateConstraintsChanged(bool autoCreate);
    void edgeComplianceChanged(float compliance);
    void bendComplianceChanged(float compliance);
    void topologyChanged();

protected:
    virtual bool usesRenderingTextureCoordinates() const;
    virtual void cacheRestPositionsForRendering(const QVector<QVector3D> &restPositions);
    virtual QVector2D textureCoordinateForRendering(int vertexIndex,
                                                    const QVector3D &restPosition,
                                                    const QVector3D &restFaceNormal) const;

    virtual JPH::Ref<JPH::SoftBodySharedSettings> createJoltSettings() const;
    void clearTopology();
    void markTopologyChanged();
    SoftBodyVertex *topologyVertexAt(int index);

private:
    friend class SoftBody;
    friend class SoftBodyMeshGeometry;

    static void appendVertex(QQmlListProperty<SoftBodyVertex> *list, SoftBodyVertex *vertex);
    static qsizetype vertexCount(QQmlListProperty<SoftBodyVertex> *list);
    static SoftBodyVertex *vertexAt(QQmlListProperty<SoftBodyVertex> *list, qsizetype index);
    static void clearVertices(QQmlListProperty<SoftBodyVertex> *list);

    static void appendFace(QQmlListProperty<SoftBodyFace> *list, SoftBodyFace *face);
    static qsizetype faceCount(QQmlListProperty<SoftBodyFace> *list);
    static SoftBodyFace *faceAt(QQmlListProperty<SoftBodyFace> *list, qsizetype index);
    static void clearFaces(QQmlListProperty<SoftBodyFace> *list);

    static void appendEdge(QQmlListProperty<SoftBodyEdge> *list, SoftBodyEdge *edge);
    static qsizetype edgeCount(QQmlListProperty<SoftBodyEdge> *list);
    static SoftBodyEdge *edgeAt(QQmlListProperty<SoftBodyEdge> *list, qsizetype index);
    static void clearEdges(QQmlListProperty<SoftBodyEdge> *list);

    static void appendVolume(QQmlListProperty<SoftBodyVolume> *list, SoftBodyVolume *volume);
    static qsizetype volumeCount(QQmlListProperty<SoftBodyVolume> *list);
    static SoftBodyVolume *volumeAt(QQmlListProperty<SoftBodyVolume> *list, qsizetype index);
    static void clearVolumes(QQmlListProperty<SoftBodyVolume> *list);

    QList<SoftBodyVertex *> m_vertices;
    QList<SoftBodyFace *> m_faces;
    QList<SoftBodyEdge *> m_edges;
    QList<SoftBodyVolume *> m_volumes;
    bool m_autoCreateConstraints = true;
    float m_edgeCompliance = 0.0f;
    float m_bendCompliance = FLT_MAX;
};

#endif // SOFTBODYSHAREDSETTINGS_H
