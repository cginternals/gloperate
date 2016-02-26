
#include <gloperate-qt/viewer/UpdateManager.h>

#include <QTime>

#include <gloperate/viewer/ViewerContext.h>

#include <globjects/base/baselogging.h>


namespace gloperate_qt
{


UpdateManager::UpdateManager(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
    // Connect update timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &UpdateManager::onTimer
    );

    // Setup timer
    m_timer.setSingleShot(true);
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
    // Get time delta
    float delta = m_time.elapsed() / 1000.0f;
    m_time.restart();

    // Update timing
    if (m_viewerContext->update(delta))
    {
        m_timer.start(0);
    }
}


} // namespace gloperate_qt
