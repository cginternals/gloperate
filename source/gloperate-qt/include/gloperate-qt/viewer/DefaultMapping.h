
#pragma once

#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <gloperate/ext-includes-end.h>

#include <glm/glm.hpp>

#include <gloperate/navigation/AbstractMapping.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QTimer;


namespace gloperate
{
    class CoordinateProvider;
    class TypedRenderTargetCapability;
    class AbstractMetaInformationCapability;
    class AbstractViewportCapability;
    class WorldInHandNavigation;
    class KeyboardEvent;
    class MouseEvent;
    class WheelEvent;
}

namespace gloperate_qt
{

class QtOpenGLWindow;


class GLOPERATE_QT_API DefaultMapping : public QObject, public gloperate::AbstractMapping
{
    Q_OBJECT


public:
    DefaultMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~DefaultMapping();

    virtual void initializeTools() override;


protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

    void mapKeyboardEvent(gloperate::KeyboardEvent * event);
    void mapMouseEvent(gloperate::MouseEvent * event);
    void mapWheelEvent(gloperate::WheelEvent * event);

    void onTargetFramebufferChanged();


protected slots:
    void showTooltip();
    void hideTooltip();


protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
    gloperate::AbstractMetaInformationCapability * m_metaInformationCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    QTimer * m_timer;
    gloperate_qt::QtOpenGLWindow * m_window;
    glm::ivec2 m_currentMousePosition;
};


} // namespace gloperate_qt
