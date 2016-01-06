
#pragma once


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QScopedPointer>
#include <QMainWindow>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_Viewer;

namespace widgetzeug
{
    class MessageStatusWidget;
    class MessageWidget;
    class ScriptPromptWidget;
}

namespace gloperate
{
    class ResourceManager;
    class PluginManager;
    class Painter;
}


namespace gloperate_qt
{


class QtOpenGLWindow;
class DefaultMapping;
class ScriptEnvironment;
class ViewerApi;
class PluginApi;


/**
*  @brief
*    Main window for gloperate-viewer
*
*    This class contains the main window for a fully-fledged
*    viewer application. It is used by gloperate-viewer and
*    can be used as a base for your own viewer application.
*
*    It contains an OpenGL canvas that displays a gloperate::Painter,
*    a property widget to configure the painter, a console output
*    window, and a scripting console. The main menu allows for
*    the selection of a painter from the available plugins,
*    a choice of the current navigation technique, and additional tools
*    such as screenshot functionality.
*
*    For generic viewer applications, this class may be used as a
*    template. For more specific applications, a usage of the
*    individual components may be more suitable.
*
*    @see QtOpenGLWindow
*/
class GLOPERATE_QT_API Viewer : public QMainWindow
{
    Q_OBJECT


public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent widget, can be nullptr
    *  @param[in] flags
    *    Window flags
    */
    Viewer(QWidget * parent = nullptr, Qt::WindowFlags flags = 0);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Viewer();
    //@}

    //@{
    /**
    *  @brief
    *    Get OpenGL canvas
    *
    *  @return
    *    OpenGL widget (cannot be nullptr)
    */
    QtOpenGLWindow * canvas() const;
    //@}

    //@{
    /**
    *  @brief
    *    Load painter
    *
    *  @param[in] name
    *    Painter name
    *
    *  @remarks
    *    If a painter plugin with this name has been registered,
    *    the painter is created and set.
    *    Otherwise, nothing happens.
    */
    void loadPainter(const std::string & name);
    //@}

    //@{
    /**
    *  @brief
    *    Set painter
    *
    *  @param[in] painter
    *    Painter reference
    *
    *  @remarks
    *    Loads the painter into the viewer.
    */
    void setPainter(gloperate::Painter & painter);
    //@}

    //@{
    /**
    *  @brief
    *    Get scripting environment
    *
    *  @return
    *    Scripting environment
    */
    const ScriptEnvironment * scriptEnvironment() const;
    ScriptEnvironment * scriptEnvironment();
    //@}


protected:
    void setupMessageWidgets();
    void setupCommandPrompt();
    void setupPropertyWidget();
    void setupDockArea();
    void setupCanvas();
    void setupScripting();
    void updatePainterMenu();

    void deinitializePainter();


protected slots:
    void on_pluginConfigAction_triggered();
    void on_captureImageAction_triggered();
    void onPainterSelected(bool checked);
    void on_reloadShadersAction_triggered();


protected:
    const QScopedPointer<Ui_Viewer> m_ui;

    std::unique_ptr<gloperate::ResourceManager>      m_resourceManager;
    std::unique_ptr<gloperate::PluginManager>        m_pluginManager;
    std::unique_ptr<ScriptEnvironment>               m_scriptEnvironment;
    std::unique_ptr<ViewerApi>                       m_viewerApi;
    std::unique_ptr<PluginApi>                       m_pluginApi;

    std::unique_ptr<gloperate::Painter>              m_painter;
    std::unique_ptr<DefaultMapping>                  m_mapping;

    std::unique_ptr<QtOpenGLWindow>                  m_canvas;
    std::unique_ptr<widgetzeug::MessageStatusWidget> m_messagesStatus;
    std::unique_ptr<widgetzeug::MessageWidget>       m_messagesLog;
    std::unique_ptr<widgetzeug::ScriptPromptWidget>  m_scriptPrompt;

    QDockWidget                                    * m_messagLogDockWidget;
    QDockWidget                                    * m_scriptPromptDockWidget;
    QDockWidget                                    * m_propertyDockWidget;
};


} // namespace gloperate_qt
