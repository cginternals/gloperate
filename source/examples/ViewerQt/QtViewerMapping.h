#pragma once

#include <gloperate/navigation/AbstractMapping.h>

using namespace gloperate;

class QtViewerMapping : public AbstractMapping
{
public:
    QtViewerMapping();
    virtual ~QtViewerMapping();

    virtual void processEvent(AbstractEvent * event);

protected:

};
