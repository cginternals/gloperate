#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <QObject>
#include <QTimer>

#include <gloperate-qt/AbstractQtMapping.h>

#include <gloperate-qtapplication/gloperate-qtapplication_api.h>

namespace gloperate {
    class CoordinateProvider;
    class WorldInHandNavigation;
    class AbstractMetaInformationCapability;
    class AbstractTypedRenderTargetCapability;
    class AbstractViewportCapability;
}

namespace gloperate_qtapplication
{

class GLOPERATE_QTAPPLICATION_API Mapping : public QObject, public gloperate_qt::AbstractQtMapping
{
    Q_OBJECT
public:
    Mapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~Mapping();

    virtual void initializeTools() override;

protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

protected:
    std::unique_ptr<gloperate::WorldInHandNavigation> m_navigation;
    std::unique_ptr<gloperate::CoordinateProvider> m_coordProvider;
    gloperate::AbstractMetaInformationCapability * m_metaInformationCapability;
    gloperate::AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    glm::ivec2 m_currentMousePosition;
    QTimer * m_timer;

protected slots:
    void showTooltip();
    void hideTooltip();
};

} // namespace gloperate_qtapplication
