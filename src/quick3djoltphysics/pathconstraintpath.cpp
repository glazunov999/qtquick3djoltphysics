#include "pathconstraintpath_p.h"

PathConstraintPathBase::PathConstraintPathBase(QObject *parent) : QObject(parent)
{
}

PathConstraintPathBase::~PathConstraintPathBase() = default;

bool PathConstraintPathBase::isLooping() const
{
    return m_isLooping;
}

void PathConstraintPathBase::setIsLooping(bool isLooping)
{
    if (m_isLooping == isLooping)
        return;

    m_isLooping = isLooping;
    emit isLoopingChanged(m_isLooping);
    emit changed();
}
