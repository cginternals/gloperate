
#pragma once

#include <gloperate/navigation/AbstractMapping.h>

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
    class KeyboardEvent;
    class MouseEvent;
    class WheelEvent;
}

namespace gloperate_qt
{
    class QtOpenGLWindow;
}

class QtViewerMapping : public gloperate::AbstractMapping
{
public:
    QtViewerMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~QtViewerMapping();

    virtual void initializeTools() override;


protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

    void mapKeyboardEvent(gloperate::KeyboardEvent * event);
    void mapMouseEvent(gloperate::MouseEvent * event);
    void mapWheelEvent(gloperate::WheelEvent * event);

    void onTargetFramebufferChanged();


protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
    gloperate_qt::QtOpenGLWindow * m_window;
};
