#ifndef TAPEREDCYLINDERSHAPE_P_H
#define TAPEREDCYLINDERSHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT TaperedCylinderShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(float topDiameter READ topDiameter WRITE setTopDiameter NOTIFY topDiameterChanged)
    Q_PROPERTY(float bottomDiameter READ bottomDiameter WRITE setBottomDiameter NOTIFY bottomDiameterChanged)
    Q_PROPERTY(float convexRadius READ convexRadius WRITE setConvexRadius NOTIFY convexRadiusChanged)
    QML_NAMED_ELEMENT(TaperedCylinderShape)
public:
    TaperedCylinderShape(QQuick3DNode *parent = nullptr);
    ~TaperedCylinderShape() override;

    float height() const;
    void setHeight(float height);

    float topDiameter() const;
    void setTopDiameter(float topDiameter);

    float bottomDiameter() const;
    void setBottomDiameter(float bottomDiameter);

    float convexRadius() const;
    void setConvexRadius(float convexRadius);

signals:
    void heightChanged(float height);
    void topDiameterChanged(float topDiameter);
    void bottomDiameterChanged(float bottomDiameter);
    void convexRadiusChanged(float convexRadius);

protected:
    void createJoltShape() override;

private:
    float m_height = 1.0f;
    float m_topDiameter = 1.0f;
    float m_bottomDiameter = 1.0f;
    float m_convexRadius = 0.05f;
};

#endif // TAPEREDCYLINDERSHAPE_P_H
