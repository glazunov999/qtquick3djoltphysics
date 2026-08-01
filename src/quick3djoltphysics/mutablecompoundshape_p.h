#ifndef MUTABLECOMPOUNDSHAPE_P_H
#define MUTABLECOMPOUNDSHAPE_P_H

#include "abstractcompoundshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT MutableCompoundShape : public AbstractCompoundShape
{
    Q_OBJECT
    QML_NAMED_ELEMENT(MutableCompoundShape)
public:
    MutableCompoundShape(QQuick3DNode *parent = nullptr);
    ~MutableCompoundShape() override;

    Q_INVOKABLE void addShape(AbstractShape *shape);
    Q_INVOKABLE void removeShape(int index);
    Q_INVOKABLE void modifyShapes(bool updateMassProperties = true);

protected:
    void createJoltShape() override;

private:
    void notifyBodyShapeChanged(const QVector3D &previousCenterOfMass, bool updateMassProperties);
};

#endif // MUTABLECOMPOUNDSHAPE_P_H
