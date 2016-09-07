#include <gloperate/base/AbstractWindow.h>


namespace gloperate
{

AbstractWindow::AbstractWindow(ResourceManager & resourceManager)
:   m_painter(nullptr)
,   m_resourceManager(resourceManager)
{
}

gloperate::Painter * AbstractWindow::painter() const
{
    return m_painter;
}

void AbstractWindow::setPainter(gloperate::Painter * painter)
{
    if (painter != m_painter)
    {
        onPainterChanged(painter);
    }

    m_painter = painter;
}

gloperate::ResourceManager & AbstractWindow::resourceManager()
{
    return m_resourceManager;
}

const gloperate::ResourceManager & AbstractWindow::resourceManager() const
{
    return m_resourceManager;
}

} /* namespace gloperate */
