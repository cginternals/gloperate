
#include <gloperate-qtapplication/Viewer.h>

#include <cassert>

#include <QDockWidget>
#include <QLayout>
#include <QUrl>
#include <QDebug>
#include <QSettings>
#include <QSizePolicy>

#include "ui_Viewer.h"

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <gloperate-qt/QtTextureStorer.h>
#include <gloperate-qt/QtKeyEventProvider.h>
#include <gloperate-qt/QtMouseEventProvider.h>
#include <gloperate-qt/QtWheelEventProvider.h>

#ifdef GLOPERATE_ASSIMP_FOUND
    #include <gloperate-assimp/AssimpMeshLoader.h>
    #include <gloperate-assimp/AssimpSceneLoader.h>
#endif

#include <widgetzeug/MessageHandler.h>
#include <widgetzeug/MessageStatusWidget.h>
#include <widgetzeug/MessageWidget.h>
#include <widgetzeug/ScriptPromptWidget.h>
#include <widgetzeug/ECMA26251_SyntaxHighlighter.h>
#include <widgetzeug/ECMA26251_Completer.h>
#include <widgetzeug/ColorSchemePresetsWidget.h>

#include <propertyguizeug/PropertyBrowser.h>

#include <gloperate-qtapplication/Mapping.h>


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

namespace gloperate_qtapplication
{

Viewer::Viewer(QWidget * parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
, m_ui { new Ui_Viewer }
, m_resourceManager { nullptr }
, m_pluginManager { nullptr }
, m_canvas { nullptr }
, m_currentPainter { nullptr }
, m_mapping { nullptr }
, m_messagesStatus { new MessageStatusWidget() } // std::make_unique is C++14
, m_messagesLog { new MessageWidget() } // make_unique is C++14
, m_scriptPrompt { new ScriptPromptWidget() } // make_unique is C++14
, m_messagLogDockWidget { nullptr }
, m_scriptPromptDockWidget { nullptr }
, m_propertyDockWidget { nullptr }
{
    // initialize resource manager (must be done BEFORE setupCanvas)
    m_resourceManager.reset(new ResourceManager());
    m_resourceManager->addLoader(new QtTextureLoader());
    m_resourceManager->addStorer(new QtTextureStorer());

#ifdef GLOPERATE_ASSIMP_FOUND
    m_resourceManager->addLoader(new AssimpMeshLoader());
    m_resourceManager->addLoader(new AssimpSceneLoader());
#endif

    // setup UI
    attachMessageWidgets(); 

    m_ui->setupUi(this);

    setupMessageWidgets();
    setupCommandPrompt();
    setupPropertyWidget();

    tabifyDockWidget(m_messagLogDockWidget, m_scriptPromptDockWidget);

    setupCanvas();

    // restore gui related settings

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());

//    setCentralWidget(QWidget::createWindowContainer(m_canvas.get()));

    // initialize plugin manager
    PluginManager::init(QCoreApplication::applicationFilePath().toStdString());

    m_pluginManager.reset(new PluginManager()); // make_unique is C++14
#ifdef NDEBUG
    m_pluginManager->addPath("plugins");
#else
    m_pluginManager->addPath("plugins/debug");
#endif

    m_pluginManager->scan("painter");

    QMenu * menu = m_ui->pluginsMenu;
    for (auto plugin : m_pluginManager->plugins())
    {
        qDebug("->  %s %s - %s (%s by %s)", plugin->name(), plugin->version(), plugin->description(), plugin->type(), plugin->vendor());

        QAction * action = new QAction(QString::fromStdString(plugin->name()), menu);
        action->setData(reinterpret_cast<qint64>(plugin));
        connect(action, SIGNAL(toggled(bool)), this, SLOT(switchToPainter(bool)));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(switchToPainter(bool)));

        menu->addAction(action);
    }
}

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());

    MessageHandler::dettach(*m_messagesLog);
    MessageHandler::dettach(*m_messagesStatus);
}

void Viewer::attachMessageWidgets()
{
    assert(m_messagesStatus);
    assert(m_messagesLog);

    MessageHandler::attach(*m_messagesStatus);
    MessageHandler::attach(*m_messagesLog);

    MessageHandler::printsToFile(QtMsgType::QtWarningMsg);
    MessageHandler::printsToFile(QtMsgType::QtCriticalMsg);
    MessageHandler::printsToFile(QtMsgType::QtFatalMsg);

    const QString fileLog(QString("Messages are also written to file://%1.").arg(MessageHandler::fileName()));
    qDebug("%s", qPrintable(fileLog));
}

void Viewer::setupMessageWidgets()
{
    assert(m_messagesStatus);
    assert(m_messagesLog);

    statusBar()->addPermanentWidget(m_messagesStatus.get());

    m_messagLogDockWidget = new QDockWidget(tr("Message Log"));
    m_messagLogDockWidget->setWidget(m_messagesLog.get());
    m_messagLogDockWidget->setObjectName("MessageLogDockWidget");

    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_messagLogDockWidget);
    m_messagesStatus->attachWidget(m_messagLogDockWidget);

    m_messagesLog->setFrameShape(QFrame::NoFrame);
}

void Viewer::setupCommandPrompt()
{
    m_scriptPrompt->setSyntaxHighlighter(new ECMA26251SyntaxHighlight);
    m_scriptPrompt->setCompleter(new ECMA26251Completer);

    m_scriptPromptDockWidget = new QDockWidget(tr("Script Prompt"));
    m_scriptPromptDockWidget->setWidget(m_scriptPrompt.get());
    m_scriptPromptDockWidget->setObjectName("ScriptPromptWidget");

    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_scriptPromptDockWidget);

    m_scriptPrompt->setFrameShape(QFrame::NoFrame);
}

void Viewer::setupPropertyWidget()
{
    m_propertyDockWidget = new QDockWidget("Properties", this);
    m_propertyDockWidget->setObjectName("PropertyDockWidget");
    m_propertyDockWidget->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea);

    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_propertyDockWidget, Qt::Orientation::Horizontal);

    m_propertyDockWidget->show();
    m_propertyDockWidget->hide();
}

void Viewer::setupCanvas()
{
    // initialized context
    QSurfaceFormat format;

#ifdef __APPLE__
    format.setVersion(3, 2);
#else
    //format.setVersion(3, 2);
#endif
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(16);

    m_canvas.reset(new gloperate_qt::QtOpenGLWindow(*m_resourceManager, format)); // make_unique is C++14

    setCentralWidget(QWidget::createWindowContainer(m_canvas.get()));
    centralWidget()->setFocusPolicy(Qt::StrongFocus);

    gloperate_qt::QtKeyEventProvider * keyProvider = new gloperate_qt::QtKeyEventProvider();
    keyProvider->setParent(m_canvas.get());
    gloperate_qt::QtMouseEventProvider * mouseProvider = new gloperate_qt::QtMouseEventProvider();
    mouseProvider->setParent(m_canvas.get());
    gloperate_qt::QtWheelEventProvider * wheelProvider = new gloperate_qt::QtWheelEventProvider();
    wheelProvider->setParent(m_canvas.get());

    m_canvas->installEventFilter(keyProvider);
    m_canvas->installEventFilter(mouseProvider);
    m_canvas->installEventFilter(wheelProvider);

    m_mapping.reset(new Mapping(m_canvas.get()));
    m_mapping->addProvider(keyProvider);
    m_mapping->addProvider(mouseProvider);
    m_mapping->addProvider(wheelProvider);
}

void Viewer::on_colorSchemePresetsAction_triggered()
{
    // ToDo: this is just a temporary test...
    /*
    QFile file("data/colorbrewer.json");
    std::unique_ptr<ColorSchemePresetsWidget> cspw(ColorSchemePresetsWidget::fromJson(file));

    if (!cspw)
    {
        qWarning("Could not open color scheme presets from \"%s\".", qPrintable(file.fileName()));
        cspw.reset(new ColorSchemePresetsWidget());
    }

    cspw->setWindowModality(Qt::ApplicationModal);
    cspw->show();*/
}

void Viewer::on_showPluginsAction_triggered()
{
    assert(m_pluginManager);

    qDebug("#%i plugins found:", static_cast<int>(m_pluginManager->plugins().size()));
    for (auto plugin : m_pluginManager->plugins())
        qDebug("->  %s %s - %s (%s by %s)", plugin->name(), plugin->version(), plugin->description(), plugin->type(), plugin->vendor());
}

void Viewer::switchToPainter(bool)
{
    QAction * action = dynamic_cast<QAction*>(QObject::sender());

    Q_ASSERT(action != nullptr);

    gloperate::Plugin * plugin = reinterpret_cast<gloperate::Plugin * >(action->data().value<qint64>());

    Q_ASSERT(plugin);

    m_currentPainter.reset(plugin->createPainter(*m_resourceManager));

    // check for painter context format requirements
    // ToDo:

    m_canvas->setPainter(m_currentPainter.get());
    m_mapping->setPainter(m_currentPainter.get());

    m_canvas->initialize();

    if (m_currentPainter.get())
    {
        QWidget * old = m_propertyDockWidget->widget();
        delete old;

        m_propertyDockWidget->setWidget(new propertyguizeug::PropertyBrowser(m_currentPainter.get()));
        m_propertyDockWidget->show();
    }
    else
    {
        m_propertyDockWidget->hide();
    }

    m_canvas->updateGL();
}

} // namespace gloperate_qtapplication
