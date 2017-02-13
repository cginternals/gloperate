
#include <gloperate-qtquick/TextureRenderItem.h>

#include <QVariant>
#include <QColor>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>

#include <gloperate/base/AbstractCanvas.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/input.h>

#include <gloperate-qtquick/QuickView.h>
#include <gloperate-qtquick/Utils.h>

using namespace gloperate_qt;


namespace gloperate_qtquick
{


TextureRenderItem::TextureRenderItem(QQuickItem * parent)
: QQuickItem(parent)
, m_environment(nullptr)
, m_x(0.0f)
, m_y(0.0f)
, m_width(0.0f)
, m_height(0.0f)
, m_devicePixelRatio(1.0f)
, m_initialized(false)
{
    // Set input modes
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    // Connect to event when the window into which is rendered has changed
    connect(
        this, &QQuickItem::windowChanged,
        this, &TextureRenderItem::onWindowChanged,
        Qt::DirectConnection
    );
}

TextureRenderItem::~TextureRenderItem()
{
}

void TextureRenderItem::onWindowChanged(QQuickWindow * window)
{
    // Check if window is valid
    if (!window)
    {
        return;
    }

    // Get gloperate environment
    QuickView * view = static_cast<QuickView*>(window);
    m_environment = view->environment();

    // Get device/pixel-ratio
    m_devicePixelRatio = window->devicePixelRatio();

    // Connect to window draw event
    connect(
        window, &QQuickWindow::afterRendering,
        this, &TextureRenderItem::onAfterRendering,
        Qt::DirectConnection
    );
}

void TextureRenderItem::onAfterRendering()
{
    if (!window() || !isVisible())
    {
        return;
    }

    // Render into item
    render();

    // Reset OpenGL state
    window()->resetOpenGLState();
}

void TextureRenderItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    QPointF pos1 = mapToScene(newGeometry.topLeft()     - newGeometry.topLeft());
    QPointF pos2 = mapToScene(newGeometry.bottomRight() - newGeometry.topLeft());

    m_x = pos1.x();
    m_y = pos1.y();
    m_width = pos2.x() - pos1.x() + 1;
    m_height = pos2.y() - pos1.y() + 1;
}

int TextureRenderItem::getWindowHeight()
{
    return window()->height();
}

const QString & TextureRenderItem::path() const
{
    return m_path;
}

void TextureRenderItem::setPath(const QString & path)
{
    m_path    = path;
    m_texture = nullptr;
}


} // namespace gloperate_qtquick
