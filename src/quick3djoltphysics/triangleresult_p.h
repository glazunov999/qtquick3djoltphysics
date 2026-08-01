#ifndef TRIANGLERESULT_P_H
#define TRIANGLERESULT_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtCore/QObject>
#include <QtGui/QVector3D>
#include <QtQml/QQmlEngine>

#include "abstractphysicsbody_p.h"
#include "softbody_p.h"

class PhysicsMaterial;

class Q_QUICK3DJOLTPHYSICS_EXPORT TriangleResult
{
    Q_GADGET
    Q_PROPERTY(QVector3D vertex0 READ vertex0 CONSTANT)
    Q_PROPERTY(QVector3D vertex1 READ vertex1 CONSTANT)
    Q_PROPERTY(QVector3D vertex2 READ vertex2 CONSTANT)
    Q_PROPERTY(AbstractPhysicsBody *body READ body CONSTANT)
    Q_PROPERTY(SoftBody *softBody READ softBody CONSTANT)
public:
    TriangleResult();
    TriangleResult(const QVector3D &v0, const QVector3D &v1, const QVector3D &v2,
                   AbstractPhysicsBody *body, SoftBody *softBody);

    QVector3D vertex0() const;
    QVector3D vertex1() const;
    QVector3D vertex2() const;
    AbstractPhysicsBody *body() const;
    SoftBody *softBody() const;

private:
    QVector3D m_vertex0;
    QVector3D m_vertex1;
    QVector3D m_vertex2;
    AbstractPhysicsBody *m_body = nullptr;
    SoftBody *m_softBody = nullptr;
};

#endif // TRIANGLERESULT_P_H
