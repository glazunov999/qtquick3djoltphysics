#ifndef TAPEREDCAPSULEGEOMETRY_P_H
#define TAPEREDCAPSULEGEOMETRY_P_H

#include <QQuick3DGeometry>

class TaperedCapsuleGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(TaperedCapsuleGeometry)
    Q_PROPERTY(bool enableNormals READ enableNormals WRITE setEnableNormals NOTIFY enableNormalsChanged)
    Q_PROPERTY(bool enableUV READ enableUV WRITE setEnableUV NOTIFY enableUVChanged)
    Q_PROPERTY(int longitudes READ longitudes WRITE setLongitudes NOTIFY longitudesChanged)
    Q_PROPERTY(int latitudes READ latitudes WRITE setLatitudes NOTIFY latitudesChanged)
    Q_PROPERTY(int rings READ rings WRITE setRings NOTIFY ringsChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(float topDiameter READ topDiameter WRITE setTopDiameter NOTIFY topDiameterChanged)
    Q_PROPERTY(float bottomDiameter READ bottomDiameter WRITE setBottomDiameter NOTIFY bottomDiameterChanged)

public:
    TaperedCapsuleGeometry();

    bool enableNormals() const { return m_enableNormals; }
    void setEnableNormals(bool enable);

    bool enableUV() const { return m_enableUV; }
    void setEnableUV(bool enable);

    int longitudes() const { return m_longitudes; }
    void setLongitudes(int longitudes);

    int latitudes() const { return m_latitudes; }
    void setLatitudes(int latitudes);

    int rings() const { return m_rings; }
    void setRings(int rings);

    float height() const { return m_height; }
    void setHeight(float height);

    float topDiameter() const { return m_topDiameter; }
    void setTopDiameter(float topDiameter);

    float bottomDiameter() const { return m_bottomDiameter; }
    void setBottomDiameter(float bottomDiameter);

signals:
    void enableNormalsChanged();
    void enableUVChanged();
    void longitudesChanged();
    void latitudesChanged();
    void ringsChanged();
    void heightChanged();
    void topDiameterChanged();
    void bottomDiameterChanged();

private:
    enum class UvProfile { Fixed, Aspect, Uniform };

    void updateData();

    bool m_enableNormals = true;
    bool m_enableUV = false;
    int m_longitudes = 32;
    int m_latitudes = 16;
    int m_rings = 1;
    float m_height = 100.f;
    float m_topDiameter = 100.f;
    float m_bottomDiameter = 100.f;
    UvProfile m_uvProfile = UvProfile::Fixed;
};

#endif // TAPEREDCAPSULEGEOMETRY_P_H
