#ifndef TAPEREDCAPSULESHAPE_P_H
#define TAPEREDCAPSULESHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT TaperedCapsuleShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(float topDiameter READ topDiameter WRITE setTopDiameter NOTIFY topDiameterChanged)
    Q_PROPERTY(float bottomDiameter READ bottomDiameter WRITE setBottomDiameter NOTIFY bottomDiameterChanged)
    QML_NAMED_ELEMENT(TaperedCapsuleShape)
public:
    TaperedCapsuleShape(QQuick3DNode *parent = nullptr);
    ~TaperedCapsuleShape() override;

    float height() const;
    void setHeight(float height);

    float topDiameter() const;
    void setTopDiameter(float topDiameter);

    float bottomDiameter() const;
    void setBottomDiameter(float bottomDiameter);

signals:
    void heightChanged(float height);
    void topDiameterChanged(float topDiameter);
    void bottomDiameterChanged(float bottomDiameter);

protected:
    void createJoltShape() override;

private:
    float m_height = 1.0f;
    float m_topDiameter = 1.0f;
    float m_bottomDiameter = 1.0f;
};

#endif // TAPEREDCAPSULESHAPE_P_H
