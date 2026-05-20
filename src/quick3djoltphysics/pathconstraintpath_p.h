#ifndef PATHCONSTRAINTPATH_P_H
#define PATHCONSTRAINTPATH_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QObject>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Constraints/PathConstraintPath.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT PathConstraintPathBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLooping READ isLooping WRITE setIsLooping NOTIFY isLoopingChanged)
    QML_NAMED_ELEMENT(PathConstraintPath)
    QML_UNCREATABLE("abstract interface")
public:
    explicit PathConstraintPathBase(QObject *parent = nullptr);
    ~PathConstraintPathBase() override;

    bool isLooping() const;
    void setIsLooping(bool isLooping);

    virtual int pointCount() const { return 0; }

    virtual JPH::Ref<JPH::PathConstraintPath> createJoltPath() const = 0;

signals:
    void isLoopingChanged(bool isLooping);
    void changed();

protected:
    bool m_isLooping = false;
};

#endif // PATHCONSTRAINTPATH_P_H
