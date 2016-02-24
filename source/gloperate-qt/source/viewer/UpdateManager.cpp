
#include <gloperate-qt/viewer/UpdateManager.h>

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
    // Update timing
    if (m_viewerContext->update())
    {
        m_timer.start(0);
    }
}


} // namespace gloperate_qt
