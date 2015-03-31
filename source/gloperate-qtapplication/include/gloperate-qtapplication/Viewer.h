#pragma once

#include <memory>

#include <QScopedPointer>
#include <QMainWindow>

#include <gloperate-qtapplication/gloperate-qtapplication_api.h>


class Ui_Viewer;
class QSurfaceFormat;

namespace gloperate
{
	class ResourceManager;
    class PluginManager;
    class Painter;
}

namespace gloperate_qt
{
    class QtOpenGLWindow;
}

namespace widgetzeug
{
    class MessageStatusWidget;
    class MessageWidget;
    class ScriptPromptWidget;
}

namespace gloperate_qtapplication
{

class Mapping;

/** \brief ToDo
*
*
* \code{.cpp}
* Moep
* \endcode
*/
class GLOPERATE_QTAPPLICATION_API Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(QWidget * parent = nullptr, Qt::WindowFlags flags = NULL);
    virtual ~Viewer();

protected slots:
    void on_colorSchemePresetsAction_triggered();
	void on_showPluginsAction_triggered();

    void switchToPainter(bool);

protected:
	void attachMessageWidgets();
	void setupMessageWidgets();
	void setupCommandPrompt();
    void setupPropertyWidget();

	void setupCanvas();

protected:
    const QScopedPointer<Ui_Viewer> m_ui;

	//std::make_unqiue gloperate_qt::QtOpenGLWindow * canvas

	std::unique_ptr<gloperate::ResourceManager> m_resourceManager;
	std::unique_ptr<gloperate::PluginManager>   m_pluginManager;

    std::unique_ptr<gloperate_qt::QtOpenGLWindow> m_canvas;
    std::unique_ptr<gloperate::Painter> m_currentPainter;
    std::unique_ptr<Mapping> m_mapping;

	std::unique_ptr<widgetzeug::MessageStatusWidget> m_messagesStatus;
	std::unique_ptr<widgetzeug::MessageWidget>       m_messagesLog;
	std::unique_ptr<widgetzeug::ScriptPromptWidget>  m_scriptPrompt;

	QDockWidget * m_messagLogDockWidget;
	QDockWidget * m_scriptPromptDockWidget;
    QDockWidget * m_propertyDockWidget;
};

} // namespace gloperate_qtapplication
