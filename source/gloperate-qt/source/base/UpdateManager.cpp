
#include <gloperate-qt/base/UpdateManager.h>

#include <QTime>

#include <gloperate/base/Environment.h>


namespace gloperate_qt
{


UpdateManager::UpdateManager(gloperate::Environment * environment)
: m_environment(environment)
{
    // Connect update timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &UpdateManager::onTimer
    );

    // Setup timer
    m_timer.setSingleShot(false);
    m_timer.start(0);

    // Start time measurement
    m_time.start();
}

UpdateManager::~UpdateManager()
{
}

void UpdateManager::wakeTimer()
{
    m_timer.start(0);
}

void UpdateManager::onTimer()
{
    // Restart timer
    m_time.restart();

    // Update timing
    if (!m_environment->update())
    {
        m_timer.stop();
    }
}


} // namespace gloperate_qt
