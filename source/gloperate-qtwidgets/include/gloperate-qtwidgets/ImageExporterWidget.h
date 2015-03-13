#pragma once

#include <memory>

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

#include <widgetzeug/DockableScrollAreaWidget.h>

namespace gloperate
{

class ResourceManager;
class Painter;

}

namespace gloperate_qt
{

class QtOpenGLWindow;

}

namespace gloperate_qtwidgets
{

class ImageExporterOutputWidget;
class ImageExporterResolutionWidget;
class ImageExporterTilebasedWidget;

class GLOPERATE_QTWIDGETS_API ImageExporterWidget : public widgetzeug::DockableScrollAreaWidget
{
    Q_OBJECT

public:
    ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
    virtual ~ImageExporterWidget();

private:
    std::unique_ptr<ImageExporterOutputWidget> m_outputWidget;
    std::unique_ptr<ImageExporterResolutionWidget> m_resolutionWidget;
    std::unique_ptr<ImageExporterTilebasedWidget> m_tilebasedWidget;
};

} //namespace gloperate_qtwidgets
