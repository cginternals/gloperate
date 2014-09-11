#include <gloperate-qt/TimePropagator.h>

#include <chrono>

namespace gloperate_qt
{

TimePropagator::TimePropagator(gloperate_qt::QtOpenGLWindowBase* window, gloperate::VirtualTimeCapability * capability)
    : m_window(window)
    , m_timer(new QTimer())
    , m_time(false, true)
    , m_capability(capability)
{
    connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(update()));

    m_timer->setSingleShot(false);
    m_timer->start(0);
    m_time.start();
}

void TimePropagator::update()
{
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(m_time.elapsed()).count();
    m_time.reset();

    m_capability->update(delta);

    m_window->updateGL();
}

} // namespace gloperate_qt
