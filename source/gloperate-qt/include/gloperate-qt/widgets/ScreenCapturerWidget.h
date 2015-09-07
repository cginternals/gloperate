
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


/**
*  @brief
*    Ui for ScreenCapturer tool
*/
class GLOPERATE_QT_API ScreenCapturerWidget : public widgetzeug::DockableScrollAreaWidget
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Resource maanger used by viewer
    *  @param[in] painter
    *    Painter currently displayed by viewer, null_ptr is allowed
    *  @param[in] context
    *    OpenGL context of viewer, null_ptr is not allowed
    *
    *  @remarks
    *    Combine tilebased, output, resolution and datalink subwidgets to manage
    *    ScreenCapturer tool via UI. Create ScreenCapturer instance.
    */
    ScreenCapturerWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScreenCapturerWidget();


private:
    std::unique_ptr<ScreenCapturerOutputWidget> m_outputWidget;             ///< Output subwidget
    std::unique_ptr<ScreenCapturerResolutionWidget> m_resolutionWidget;     ///< Resolution subwidget
    std::unique_ptr<ScreenCapturerTilebasedWidget> m_tilebasedWidget;       ///< Tilebased subwidget
    std::unique_ptr<widgetzeug::DataLinkWidget> m_dataLinkWidget;           ///< Datalink subwidget
    std::unique_ptr<gloperate::ScreenCapturer> m_screenCapturer;            ///< ScreenCapturer instance for the UI
};


} // namespace gloperate_qt
