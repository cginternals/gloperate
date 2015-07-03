
#include <gloperate-qt/viewer/Viewer.h>

#include <cassert>

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
#include <gloperate/plugin/Plugin.h>

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


using namespace widgetzeug;
using namespace gloperate;
using namespace gloperate_qt;

#ifdef GLOPERATE_ASSIMP_FOUND
    using namespace gloperate_assimp;
#endif


namespace
{
    const QString SETTINGS_GEOMETRY("Geometry");
    const QString SETTINGS_STATE("State");
}


namespace gloperate_qt
{


Viewer::Viewer(QWidget * parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
, m_ui{new Ui_Viewer}
, m_resourceManager{nullptr}
, m_pluginManager {nullptr}
, m_canvas{nullptr}
, m_painter{nullptr}
, m_mapping{nullptr}
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
    PluginManager::init(QCoreApplication::applicationFilePath().toStdString());

    // Add default plugin directories
    m_pluginManager.reset(new PluginManager());
#ifdef NDEBUG
    m_pluginManager->addPath("plugins");
#else
    m_pluginManager->addPath("plugins/debug");
#endif

    // Scan all plugins with name component 'painter'
    m_pluginManager->scan("painter");

    // Update plugins-menu
    QMenu * menu = m_ui->pluginsMenu;
    for (auto plugin : m_pluginManager->plugins())
    {
        qDebug("->  %s %s - %s (%s by %s)", plugin->name(), plugin->version(), plugin->description(), plugin->type(), plugin->vendor());

        QAction * action = new QAction(QString::fromStdString(plugin->name()), menu);
        action->setData(reinterpret_cast<qint64>(plugin));
        connect(action, SIGNAL(toggled(bool)), this, SLOT(on_painter_selected(bool)));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(on_painter_selected(bool)));

        menu->addAction(action);
    }
}

Viewer::~Viewer()
{
    // Save settings
    QSettings settings;
    settings.setValue(SETTINGS_GEOMETRY, saveGeometry());
    settings.setValue(SETTINGS_STATE, saveState());

    // Disconnect message handlers
    MessageHandler::dettach(*m_messagesLog);
    MessageHandler::dettach(*m_messagesStatus);
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

    // Connect to input from scripting console
    connect(m_scriptPrompt.get(), &widgetzeug::ScriptPromptWidget::evaluate, [this] (const QString & command) {
        // Execute commands that have been typed on the scripting console
        qDebug() << "cmd: " << command;
    });
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
#else
    // Get newest available core context
#endif
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(16);

    // Create OpenGL context and window
    m_canvas.reset(new gloperate_qt::QtOpenGLWindow(*m_resourceManager, format));

    // Create widget container
    setCentralWidget(QWidget::createWindowContainer(m_canvas.get()));
    centralWidget()->setFocusPolicy(Qt::StrongFocus);

    // Setup event provider to rranslate Qt messages into gloperate events
    gloperate_qt::QtKeyEventProvider * keyProvider = new gloperate_qt::QtKeyEventProvider();
    keyProvider->setParent(m_canvas.get());
    gloperate_qt::QtMouseEventProvider * mouseProvider = new gloperate_qt::QtMouseEventProvider();
    mouseProvider->setParent(m_canvas.get());
    gloperate_qt::QtWheelEventProvider * wheelProvider = new gloperate_qt::QtWheelEventProvider();
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

void Viewer::on_painter_selected(bool /*checked*/)
{
    // Get selected menu action
    QAction * action = dynamic_cast<QAction*>(QObject::sender());
    Q_ASSERT(action != nullptr);

    // Get plugin that belongs to the menu item
    gloperate::Plugin * plugin = reinterpret_cast<gloperate::Plugin * >(action->data().value<qint64>());
    Q_ASSERT(plugin);

    // Create new painter
    m_painter.reset(plugin->createPainter(*m_resourceManager));

    // [TODO] Check for painter context format requirements

    // Setup new painter
    m_canvas->setPainter(m_painter.get());
    m_mapping->setPainter(m_painter.get());
    m_canvas->initialize();

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


} // namespace gloperate_qt
