#ifndef EXAMPLESOFTBODYSPHERESETTINGS_H
#define EXAMPLESOFTBODYSPHERESETTINGS_H

#include <QtQuick3DJoltPhysics/softbodysharedsettings.h>

class ExampleSoftBodySphereSettings : public SoftBodySharedSettings
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int numTheta READ numTheta WRITE setNumTheta NOTIFY numThetaChanged)
    Q_PROPERTY(int numPhi READ numPhi WRITE setNumPhi NOTIFY numPhiChanged)
public:
    explicit ExampleSoftBodySphereSettings(QObject *parent = nullptr);

    float radius() const;
    void setRadius(float radius);

    int numTheta() const;
    void setNumTheta(int numTheta);

    int numPhi() const;
    void setNumPhi(int numPhi);

    Q_INVOKABLE void buildSphere(float radius = -1.0f, int numTheta = -1, int numPhi = -1);

signals:
    void radiusChanged();
    void numThetaChanged();
    void numPhiChanged();

protected:
    virtual void customizeAfterBuild();

private:
    void rebuildSphere();

    float m_radius = 1.0f;
    int m_numTheta = 10;
    int m_numPhi = 20;
};

#endif // EXAMPLESOFTBODYSPHERESETTINGS_H
