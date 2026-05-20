#ifndef EXAMPLESOFTBODYCLOTHSETTINGS_H
#define EXAMPLESOFTBODYCLOTHSETTINGS_H

#include <QtQuick3DJoltPhysics/softbodysharedsettings.h>

class ExampleSoftBodyClothSettings : public SoftBodySharedSettings
{
    Q_OBJECT
    Q_PROPERTY(int gridSizeX READ gridSizeX WRITE setGridSizeX NOTIFY gridSizeXChanged)
    Q_PROPERTY(int gridSizeZ READ gridSizeZ WRITE setGridSizeZ NOTIFY gridSizeZChanged)
    Q_PROPERTY(float gridSpacing READ gridSpacing WRITE setGridSpacing NOTIFY gridSpacingChanged)
    Q_PROPERTY(bool pinTwoFrontCorners READ pinTwoFrontCorners WRITE setPinTwoFrontCorners NOTIFY pinTwoFrontCornersChanged)
    Q_PROPERTY(bool pinFourCorners READ pinFourCorners WRITE setPinFourCorners NOTIFY pinFourCornersChanged)
public:
    explicit ExampleSoftBodyClothSettings(QObject *parent = nullptr);

    int gridSizeX() const;
    void setGridSizeX(int gridSizeX);

    int gridSizeZ() const;
    void setGridSizeZ(int gridSizeZ);

    float gridSpacing() const;
    void setGridSpacing(float gridSpacing);

    bool pinTwoFrontCorners() const;
    void setPinTwoFrontCorners(bool pin);

    bool pinFourCorners() const;
    void setPinFourCorners(bool pin);

    Q_INVOKABLE void buildCloth(int gridSizeX = -1, int gridSizeZ = -1, float gridSpacing = -1.0f);

signals:
    void gridSizeXChanged();
    void gridSizeZChanged();
    void gridSpacingChanged();
    void pinTwoFrontCornersChanged();
    void pinFourCornersChanged();

protected:
    virtual void customizeAfterBuild();

private:
    void rebuildCloth();

    int m_gridSizeX = 30;
    int m_gridSizeZ = 30;
    float m_gridSpacing = 0.75f;
    bool m_pinTwoFrontCorners = false;
    bool m_pinFourCorners = false;
};

#endif // EXAMPLESOFTBODYCLOTHSETTINGS_H
