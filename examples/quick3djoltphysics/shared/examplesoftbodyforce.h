#ifndef EXAMPLESOFTBODYFORCE_H
#define EXAMPLESOFTBODYFORCE_H

#include <QObject>
#include <QVector3D>

class ExampleSoftBodyForce : public QObject
{
    Q_OBJECT
public:
    explicit ExampleSoftBodyForce(QObject *parent = nullptr);

    Q_INVOKABLE QVector3D fluctuatingForce(float time, float maxForce = 15.0f, float maxAngleDegrees = 90.0f) const;
};

#endif // EXAMPLESOFTBODYFORCE_H
