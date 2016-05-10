
#pragma once

#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <gloperate/ext-includes-end.h>

#include <glm/glm.hpp>

#include <gloperate-qt/viewer/AbstractQtMapping.h>


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

class TreeNavigation;

namespace gloperate_qt{

class GLOPERATE_QT_API TreeNavigationMapping : public QObject, public gloperate_qt::AbstractQtMapping
{
    Q_OBJECT


public:
    TreeNavigationMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~TreeNavigationMapping();

    virtual void initializeTools() override;


protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

    void mapKeyboardEvent(gloperate::KeyboardEvent * event);
    void mapMouseEvent(gloperate::MouseEvent * event);
    void mapWheelEvent(gloperate::WheelEvent * event);

    void onTargetFramebufferChanged();

protected:
    std::unique_ptr<TreeNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    glm::ivec2 m_currentMousePosition;
};

}
