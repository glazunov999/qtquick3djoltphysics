#ifndef EXAMPLESOFTBODYKINEMATICSPHERESETTINGS_H
#define EXAMPLESOFTBODYKINEMATICSPHERESETTINGS_H

#include "examplesoftbodyspheresettings.h"

class ExampleSoftBodyKinematicSphereSettings : public ExampleSoftBodySphereSettings
{
    Q_OBJECT
public:
    explicit ExampleSoftBodyKinematicSphereSettings(QObject *parent = nullptr);

protected:
    void customizeAfterBuild() override;
};

#endif // EXAMPLESOFTBODYKINEMATICSPHERESETTINGS_H
