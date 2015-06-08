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
    class TrackballNavigation;
    class KeyboardEvent;
    class MouseEvent;
    class WheelEvent;
}

class QtViewerMapping : public gloperate_qt::AbstractQtMapping
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
    enum class NavigationType
    {
        WorldInHand,
        Trackball
    };

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_worldNavigation;
    std::unique_ptr<gloperate::TrackballNavigation> m_trackballNavigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
    NavigationType m_currentNavigation;
};
