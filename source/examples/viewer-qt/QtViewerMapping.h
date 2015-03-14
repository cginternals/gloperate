#pragma once

#include <gloperate/navigation/AbstractMapping.h>

#include <memory>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
}

class QtViewerMapping : public gloperate::AbstractMapping
{
public:
    QtViewerMapping();
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
};
