#pragma once

#include <gloperate-qt/AbstractQtMapping.h>

#include <memory>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
}

class QtViewerMapping : public gloperate_qt::AbstractQtMapping
{
public:
    QtViewerMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
};
