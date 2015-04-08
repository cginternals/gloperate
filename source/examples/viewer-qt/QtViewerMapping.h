#pragma once

#include <gloperate-qt/AbstractQtMapping.h>

#include <memory>


namespace globjects
{ 
    class Framebuffer;
}

namespace gloperate
{
    class CoordinateProvider;
    class TypedRenderTargetCapability;
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

    void onTargetFramebufferChanged();

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
};
