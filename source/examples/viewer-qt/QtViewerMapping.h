#pragma once

#include <gloperate/navigation/AbstractMapping.h>

#include <memory>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
}

using namespace gloperate;

class QtViewerMapping : public AbstractMapping
{
public:
    QtViewerMapping();
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(AbstractEvent * event) override;

protected:
    std::unique_ptr<WorldInHandNavigation> m_navigation;
    std::unique_ptr<CoordinateProvider> m_coordProvider;
};
