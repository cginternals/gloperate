
#include <gloperate-qt/viewer/TimePropagator.h>

#include <chrono>


namespace gloperate_qt
{


TimePropagator::TimePropagator(gloperate_qt::QtOpenGLWindowBase * window)
:   m_window(window)
,   m_capability(nullptr)
,   m_timer(new QTimer())
,   m_time(false, true)
{
    // Connect to timer signal
    connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(update()));

    // Start timer
    m_timer->setSingleShot(false);
    m_timer->start(0);
}

void TimePropagator::setCapability(gloperate::AbstractVirtualTimeCapability * capability)
{
    m_capability = capability;
    
    if (m_capability)
        m_time.start();
    else
        m_time.stop();
}

void TimePropagator::update()
{
    if (!m_capability)
    {
        m_window->updateGL();
        return;
    }
    
    // Calculate time delta
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(m_time.elapsed()).count();
    m_time.reset();
    
    // Check if virtual time capability is enabled
    if (m_capability->enabled())
    {
        // Propagate new time
        m_capability->update(delta);
        
        // Update window
        m_window->updateGL();
    }
}


} // namespace gloperate_qt
