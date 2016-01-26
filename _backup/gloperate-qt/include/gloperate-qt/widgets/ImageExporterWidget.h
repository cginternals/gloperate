
#pragma once


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <gloperate/ext-includes-end.h>

#include <widgetzeug/DockableScrollAreaWidget.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace widgetzeug
{
    class DataLinkWidget;
}

namespace gloperate
{
    class ResourceManager;
    class Painter;
}


namespace gloperate_qt
{


class QtOpenGLWindow;
class ImageExporterMultiFrameWidget;
class ImageExporterOutputWidget;
class ImageExporterResolutionWidget;
class ImageExporterTilebasedWidget;


class GLOPERATE_QT_API ImageExporterWidget : public widgetzeug::DockableScrollAreaWidget
{
    Q_OBJECT


public:
    ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
    virtual ~ImageExporterWidget();


private:
    std::unique_ptr<ImageExporterOutputWidget> m_outputWidget;
    std::unique_ptr<ImageExporterResolutionWidget> m_resolutionWidget;
    std::unique_ptr<ImageExporterMultiFrameWidget> m_multiFrameWidget;
    std::unique_ptr<ImageExporterTilebasedWidget> m_tilebasedWidget;
    std::unique_ptr<widgetzeug::DataLinkWidget> m_dataLinkWidget;
};


} // namespace gloperate_qt
