
#include <gloperate-qt/viewer/Viewer.h>

#include <cassert>

#include <reflectionzeug/Object.h>

#include <gloperate/ext-includes-begin.h>

#include <QDockWidget>
#include <QDebug>
#include <QSettings>

#include <propertyguizeug/PropertyBrowser.h>
#include <widgetzeug/MessageHandler.h>
#include <widgetzeug/MessageStatusWidget.h>
#include <widgetzeug/MessageWidget.h>
#include <widgetzeug/ScriptPromptWidget.h>
#include <widgetzeug/ECMA26251_SyntaxHighlighter.h>
#include <widgetzeug/ECMA26251_Completer.h>

#include "ui_Viewer.h"

#include <gloperate/ext-includes-end.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/PainterPlugin.h>

#ifdef GLOPERATE_ASSIMP_FOUND
    #include <gloperate-assimp/AssimpMeshLoader.h>
    #include <gloperate-assimp/AssimpSceneLoader.h>
#endif

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/viewer/QtTextureLoader.h>
#include <gloperate-qt/viewer/QtTextureStorer.h>
#include <gloperate-qt/viewer/QtKeyEventProvider.h>
#include <gloperate-qt/viewer/QtMouseEventProvider.h>
#include <gloperate-qt/viewer/QtWheelEventProvider.h>
#include <gloperate-qt/viewer/DefaultMapping.h>
#include <gloperate-qt/widgets/ImageExporterWidget.h>
#include <gloperate-qt/scripting/ScriptEnvironment.h>
#include <gloperate-qt/scripting/SystemApi.h>
#include <gloperate-qt/scripting/TimerApi.h>
#include <gloperate-qt/scripting/ViewerApi.h>
#include <gloperate-qt/scripting/PluginApi.h>


using namespace widgetzeug;
using namespace gloperate;

#ifdef GLOPERATE_ASSIMP_FOUND
    using namespace gloperate_assimp;
#endif


namespace
{
    const QString SETTINGS_GEOMETRY("Geometry");
    const QString SETTINGS_STATE("State");
    const QString SETTINGS_PLUGINS("Plugins");

    QStringList toQStringList(const std::vector<std::string> & list)
    {
        QStringList qlist;
        for (auto s : list) {
            qlist << QString::fromStdString(s);
        }
        return qlist;
    }

    std::vector<std::string> fromQStringList(const QStringList & qlist)
    {
        std::vector<std::string> list;
        for (QString s : qlist) {
            list.push_back(s.toStdString());
        }
        return list;
    }
}


namespace gloperate_qt
{


Viewer::Viewer(QWidget * parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
, m_ui{new Ui_Viewer}
, m_resourceManager{nullptr}
, m_pluginManager{nullptr}
, m_scriptEnvironment{nullptr}
, m_viewerApi{nullptr}
, m_pluginApi{nullptr}
, m_painter{nullptr}
, m_mapping{nullptr}
, m_canvas{nullptr}
, m_messagesStatus{new MessageStatusWidget()}
, m_messagesLog{new MessageWidget()}
, m_scriptPrompt{new ScriptPromptWidget()}
, m_messagLogDockWidget{nullptr}
, m_scriptPromptDockWidget{nullptr}
, m_propertyDockWidget{nullptr}
{
    // Initialize resource manager (must be done BEFORE setupCanvas)
    m_resourceManager.reset(new ResourceManager());

    // Add default texture loaders/storers
    m_resourceManager->addLoader(new QtTextureLoader());
    m_resourceManager->addStorer(new QtTextureStorer());

    // Add assimp loaders (if available)
#ifdef GLOPERATE_ASSIMP_FOUND
    m_resourceManager->addLoader(new AssimpMeshLoader());
    m_resourceManager->addLoader(new AssimpSceneLoader());
#endif

    // Setup UI
    m_ui->setupUi(this);
    setupMessageWidgets();
    setupCommandPrompt();
    setupPropertyWidget();
    setupDockArea();
    setupCanvas();

    // Load settings
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    // Restore GUI state from settings
    restoreGeometry(settings.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(settings.value(SETTINGS_STATE).toByteArray());

    // Initialize plugin manager
    m_pluginManager.reset(new PluginManager());
    m_pluginManager->pluginsChanged.connect(this, &Viewer::updatePainterMenu);

    // Restore plugin paths from settings
    auto paths = fromQStringList(settings.value(SETTINGS_PLUGINS).toStringList());
    if (paths.size() > 0) {
        m_pluginManager->setPaths(paths);
    }

    // Add default plugin directories
    m_pluginManager->addPath(QCoreApplication::applicationDirPath().toStdString());
    m_pluginManager->addPath("plugins");

    // Scan all plugins with name component 'painters'
    #ifdef NDEBUG
        m_pluginManager->scan("painters");
    #else
        m_pluginManager->scan("paintersd");
    #endif

    // Setup scripting context
    setupScripting();
}

Viewer::~Viewer()
{
    // Save settings
    QSettings settings;
    settings.setValue(SETTINGS_GEOMETRY, saveGeometry());
    settings.setValue(SETTINGS_STATE, saveState());
    settings.setValue(SETTINGS_PLUGINS, toQStringList(m_pluginManager->paths()));

    // Disconnect message handlers
    MessageHandler::dettach(*m_messagesLog);
    MessageHandler::dettach(*m_messagesStatus);
}

QtOpenGLWindow * Viewer::canvas() const
{
    return m_canvas.get();
}

void Viewer::loadPainter(const std::string & name)
{
    // Get plugin by name
    Plugin * plugin = m_pluginManager->plugin(name);
    AbstractPainterPlugin * painterPlugin = plugin ? dynamic_cast<AbstractPainterPlugin *>(plugin) : nullptr;
    if (!painterPlugin) {
        return;
    }

    // Unload old painter
    if (m_painter.get()) {
        m_scriptEnvironment->removeScriptApi(m_painter.get());
    }

    // Create new painter
    m_painter.reset(painterPlugin->createPainter(*m_resourceManager));

    // [TODO] Check for painter context format requirements

    // Setup new painter
    m_canvas->setPainter(m_painter.get());
    m_mapping->setPainter(m_painter.get());
    m_canvas->initialize();

    // Register painter in scripting
    m_scriptEnvironment->addScriptApi(m_painter.get());

    // Update property browser
    if (m_painter.get())
    {
        QWidget * old = m_propertyDockWidget->widget();
        delete old;

        m_propertyDockWidget->setWidget(new propertyguizeug::PropertyBrowser(m_painter.get()));
        m_propertyDockWidget->show();
    }
    else
    {
        m_propertyDockWidget->hide();
    }

    // Update rendering
    m_canvas->updateGL();
}

const ScriptEnvironment * Viewer::scriptEnvironment() const
{
    return m_scriptEnvironment.get();
}

ScriptEnvironment * Viewer::scriptEnvironment()
{
    return m_scriptEnvironment.get();
}

void Viewer::setupMessageWidgets()
{
    // Widgets have to be created beforehand
    assert(m_messagesStatus);
    assert(m_messagesLog);

    // Attach message handlers to log widgets
    MessageHandler::attach(*m_messagesStatus);
    MessageHandler::attach(*m_messagesLog);

    // Additionally write log to file
    MessageHandler::printsToFile(QtMsgType::QtWarningMsg);
    MessageHandler::printsToFile(QtMsgType::QtCriticalMsg);
    MessageHandler::printsToFile(QtMsgType::QtFatalMsg);

    // Announce log initialization
    qDebug("Initialize message log.");
    const QString fileLog(QString("Messages are also written to file://%1.").arg(MessageHandler::fileName()));
    qDebug("%s", qPrintable(fileLog));

    // Create dock window for message log
    m_messagLogDockWidget = new QDockWidget(tr("Message Log"));
    m_messagLogDockWidget->setWidget(m_messagesLog.get());
    m_messagLogDockWidget->setObjectName("MessageLogDockWidget");
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_messagLogDockWidget);
    m_messagesLog->setFrameShape(QFrame::NoFrame);

    // Add item to status bar and connect to window
    statusBar()->addPermanentWidget(m_messagesStatus.get());
    m_messagesStatus->attachWidget(m_messagLogDockWidget);
}

void Viewer::setupCommandPrompt()
{
    // Widgets have to be created beforehand
    assert(m_scriptPrompt);

    // Setup syntax highlighting and auto-completion
    m_scriptPrompt->setSyntaxHighlighter(new ECMA26251SyntaxHighlight);
    m_scriptPrompt->setCompleter(new ECMA26251Completer);

    // Create dock window for scripting console
    m_scriptPromptDockWidget = new QDockWidget(tr("Script Prompt"));
    m_scriptPromptDockWidget->setWidget(m_scriptPrompt.get());
    m_scriptPromptDockWidget->setObjectName("ScriptPromptWidget");
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_scriptPromptDockWidget);
    m_scriptPrompt->setFrameShape(QFrame::NoFrame);
}

void Viewer::setupPropertyWidget()
{
    // Create dock window for property browser
    m_propertyDockWidget = new QDockWidget("Properties", this);
    m_propertyDockWidget->setObjectName("PropertyDockWidget");
    m_propertyDockWidget->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_propertyDockWidget, Qt::Orientation::Horizontal);
}

void Viewer::setupDockArea()
{
    // Define order of widgets in dock area
    tabifyDockWidget(m_messagLogDockWidget, m_scriptPromptDockWidget);
}

void Viewer::setupCanvas()
{
    // Setup OpenGL context format
    QSurfaceFormat format;
#ifdef __APPLE__
    // Get OpenGL 3.2/4.1 core context
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    // Get newest available compatibility context
#endif
    format.setDepthBufferSize(16);

    // Create OpenGL context and window
    m_canvas.reset(new QtOpenGLWindow(*m_resourceManager, format));

    // Create widget container
    setCentralWidget(QWidget::createWindowContainer(m_canvas.get()));
    centralWidget()->setFocusPolicy(Qt::StrongFocus);

    // Setup event provider to translate Qt messages into gloperate events
    QtKeyEventProvider * keyProvider = new QtKeyEventProvider();
    keyProvider->setParent(m_canvas.get());
    QtMouseEventProvider * mouseProvider = new QtMouseEventProvider();
    mouseProvider->setParent(m_canvas.get());
    QtWheelEventProvider * wheelProvider = new QtWheelEventProvider();
    wheelProvider->setParent(m_canvas.get());

    m_canvas->installEventFilter(keyProvider);
    m_canvas->installEventFilter(mouseProvider);
    m_canvas->installEventFilter(wheelProvider);

    // Create input mapping for gloperate interaction techniques
    m_mapping.reset(new DefaultMapping(m_canvas.get()));
    m_mapping->addProvider(keyProvider);
    m_mapping->addProvider(mouseProvider);
    m_mapping->addProvider(wheelProvider);
}

void Viewer::setupScripting()
{
    // Widgets have to be created beforehand
    assert(m_scriptPrompt);

    // Create scripting environment
    m_scriptEnvironment.reset(new ScriptEnvironment);

    // Register additional scripting APIs
    m_viewerApi.reset(new ViewerApi(this));
    m_scriptEnvironment->addScriptApi(m_viewerApi.get());

    m_pluginApi.reset(new PluginApi(m_pluginManager.get()));
    m_scriptEnvironment->addScriptApi(m_pluginApi.get());

    // Connect scripting environment to scripting console
    m_scriptEnvironment->connectScriptingConsole(m_scriptPrompt.get());

    // Set help text
    std::string helpText =
        "Available commands:\n"
        "  help():     Print this help message\n"
        "  print():    Print the content of a variable\n"
        "  toString(): Convert a variable into a string\n"
        "  exit():     Exit the application\n"
        "\n"
        "APIs:\n"
        "  viewer:        Viewer main window control\n"
        "  pluginManager: Plugin management\n"
        "  system:        System API (e.g., file io, keyboard handling)\n"
        "  timer:         Timer API\n"
        "\n"
        "Examples:\n"
        "  viewer.loadPainter(\"CubeScape\");\n"
        "  print(system);\n"
        "  timer.start(1000, function() { print(\"Hello Scripting World.\"); } );\n"
        "  timer.stopAll();\n";
    m_scriptEnvironment->setHelpText(helpText);
}

void Viewer::updatePainterMenu()
{
    // Clear painter menu
    QMenu * menu = m_ui->painterMenu;
    menu->clear();

    // Add all loaded plugins
    for (auto plugin : m_pluginManager->plugins())
    {
        // Add action
        QAction * action = new QAction(QString::fromStdString(plugin->name()), menu);
        action->setData(QString::fromStdString(plugin->name()));
        connect(action, SIGNAL(toggled(bool)), this, SLOT(onPainterSelected(bool)));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(onPainterSelected(bool)));

        // Add to menu
        menu->addAction(action);
    }
}

void Viewer::on_captureImageAction_triggered()
{
    // Screenshot dialog needs an active painter
    if (m_painter)
    {
        // Show image exporter dialog
        ImageExporterWidget * ie{ new ImageExporterWidget(*m_resourceManager, m_canvas->painter(), m_canvas.get()) };
        ie->setWindowModality(Qt::NonModal);
        ie->show();
    }
}

void Viewer::on_managePluginsAction_triggered()
{
    assert(m_pluginManager);

    // [TODO] Show plugin manager widget
}

void Viewer::onPainterSelected(bool /*checked*/)
{
    // Get selected menu action
    QAction * action = dynamic_cast<QAction*>(QObject::sender());
    Q_ASSERT(action != nullptr);

    // Get painter name
    QString name = action->data().toString();
    loadPainter(name.toStdString());
}


} // namespace gloperate_qt
