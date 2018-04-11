
#include <gloperate-qtquick/Application.h>

#include <QUrl>
#include <QQmlContext>
#include <QSurfaceFormat>

#include <cppassist/cmdline/ArgumentParser.h>

#include <qmltoolbox/Application.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/TimerManager.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>

#include <gloperate-qtquick/QmlScriptContext.h>


namespace gloperate_qtquick
{


void Application::initialize()
{
    qmltoolbox::Application::initialize();
}

Application::Application(int & argc, char ** argv)
: QGuiApplication(argc, argv)
, m_environment()
, m_qmlEngine(&m_environment)
{
    // Read command line options
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);

    const auto contextFormat = argumentParser.value("--context");

    if (argumentParser.isSet("-safemode"))
    {
        m_environment.setSafeMode(true);
    }

    // Create scripting context
    m_environment.setupScripting(
        cppassist::make_unique<gloperate_qtquick::QmlScriptContext>(&m_qmlEngine)
    );

    // Configure plugin paths
    m_environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );

    // Specify desired context format
    gloperate::GLContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(gloperate::GLContextFormat::Profile::Core);
    format.setForwardCompatible(true);

    if (!contextFormat.empty())
    {
        if (!format.initializeFromString(contextFormat))
        {
            // [TODO] Error: Invalid context string
        }
    }

    // Convert and set Qt context format
    QSurfaceFormat qFormat = gloperate_qt::GLContextFactory::toQSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(qFormat);

    // Pass additional command line parameters on to the QML
    QStringList paramsList;
    auto params = argumentParser.params();
    for (auto param : params) paramsList << QString::fromStdString(param);

    m_qmlEngine.rootContext()->setContextProperty("commandLineParams", paramsList);

    // Create global timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &Application::onTimer
    );

    m_timer.start(5);
}

Application::~Application()
{
}

const QString & Application::gloperateModulePath() const
{
    return m_qmlEngine.gloperateModulePath();
}

void Application::loadQml(const QString & path)
{
    m_qmlEngine.load(QUrl::fromLocalFile(path));
}

const QmlEngine & Application::qmlEngine() const
{
    return m_qmlEngine;
}

QmlEngine & Application::qmlEngine()
{
    return m_qmlEngine;
}

const gloperate::Environment & Application::environment() const
{
    return m_environment;
}

gloperate::Environment & Application::environment()
{
    return m_environment;
}

void Application::onTimer()
{
    // Update scripting timers
    m_environment.timerManager()->update();
}


} // namespace gloperate_qtquick
