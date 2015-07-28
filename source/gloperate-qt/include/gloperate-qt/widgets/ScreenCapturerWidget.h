
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
class ScreenCapturer;

}


namespace gloperate_qt
{


class QtOpenGLWindow;
class ScreenCapturerOutputWidget;
class ScreenCapturerResolutionWidget;
class ScreenCapturerTilebasedWidget;


class GLOPERATE_QT_API ScreenCapturerWidget : public widgetzeug::DockableScrollAreaWidget
{
    Q_OBJECT


public:
    ScreenCapturerWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
    virtual ~ScreenCapturerWidget();


private:
    std::unique_ptr<ScreenCapturerOutputWidget> m_outputWidget;
    std::unique_ptr<ScreenCapturerResolutionWidget> m_resolutionWidget;
    std::unique_ptr<ScreenCapturerTilebasedWidget> m_tilebasedWidget;
    std::unique_ptr<widgetzeug::DataLinkWidget> m_dataLinkWidget;
    std::unique_ptr<gloperate::ScreenCapturer> m_screenCapturer;
};


} // namespace gloperate_qt
