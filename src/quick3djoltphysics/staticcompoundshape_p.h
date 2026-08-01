#ifndef STATICCOMPOUNDSHAPE_P_H
#define STATICCOMPOUNDSHAPE_P_H

#include "abstractcompoundshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT StaticCompoundShape : public AbstractCompoundShape
{
    Q_OBJECT
    QML_NAMED_ELEMENT(StaticCompoundShape)
public:
    StaticCompoundShape(QQuick3DNode *parent = nullptr);
    ~StaticCompoundShape() override;

protected:
    void createJoltShape() override;
};

#endif // STATICCOMPOUNDSHAPE_P_H
