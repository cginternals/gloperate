
#include <gloperate-qtquick/RenderItem.h>

#include <glbinding/Binding.h>

#include <globjects/base/baselogging.h>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-qtquick/QuickView.h>


namespace gloperate_qtquick
{


RenderItem::RenderItem()
: m_renderStage(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &RenderItem::onWindowChanged, Qt::DirectConnection);
}

RenderItem::~RenderItem()
{
}

void RenderItem::onWindowChanged(QQuickWindow * window)
{
    if (!window)
    {
        return;
    }

    globjects::info() << "onWindowChanged()";

    QuickView * view = static_cast<QuickView*>(window);
    if (view)
    {
        m_renderStage = view->renderStage();
    }

    window->setClearBeforeRendering(false);

    connect(window, &QQuickWindow::beforeRendering, this, &RenderItem::onBeforeRendering, Qt::DirectConnection);
    connect(window, &QQuickWindow::beforeSynchronizing, this, &RenderItem::onBeforeSynchronizing, Qt::DirectConnection);
    connect(window, &QQuickWindow::sceneGraphInvalidated, this, &RenderItem::onSceneGraphInvalidated, Qt::DirectConnection);
}

void RenderItem::onBeforeRendering()
{
    if (!m_renderStage)
    {
        return;
    }

    globjects::info() << "onBeforeRendering()";

    m_renderStage->setDeviceViewport(0, 0, 1280, 720);
    m_renderStage->setVirtualViewport(0, 0, 1280, 720);
    m_renderStage->setFrameCounter(0);
    m_renderStage->setTimeDelta(0.01f);

    m_renderStage->process(nullptr);
}

void RenderItem::onBeforeSynchronizing()
{
    globjects::info() << "onBeforeSynchronizing()";
}

void RenderItem::onSceneGraphInvalidated()
{
    globjects::info() << "onSceneGraphInvalidated()";
}


} // namespace gloperate_qtquick
