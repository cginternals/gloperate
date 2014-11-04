#pragma once

#include <gloperate/navigation/AbstractMapping.h>

#include <memory>

namespace gloperate {
    class WorldInHandNavigation;
}

using namespace gloperate;

class QtViewerMapping : public AbstractMapping
{
public:
    QtViewerMapping();
    virtual ~QtViewerMapping();

    virtual void initializeNavigation() override;
    virtual void processEvent(AbstractEvent * event) override;

protected:
    std::unique_ptr<WorldInHandNavigation> m_navigation;
};
