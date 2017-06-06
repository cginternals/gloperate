
#include <gloperate-qt/base/Application.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/TimerManager.h>


namespace gloperate_qt
{


Application::Application(gloperate::Environment * environment, int & argc, char ** argv)
: QApplication(argc, argv)
, m_environment(environment)
{
    // Connect to exit-signal
    m_environment->exitApplication.connect([] (int exitCode)
    {
        QApplication::exit(exitCode);
    });

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

void Application::onTimer()
{
    // Update scripting timers
    m_environment->timerManager()->update();
}


} // namespace gloperate_qt
