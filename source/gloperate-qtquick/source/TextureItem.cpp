
#include <gloperate-qtquick/TextureItem.h>

#include <QQmlContext>
#include <QQuickWindow>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/Utils.h>

#include <gloperate-qtquick/TextureItemRenderer.h>


namespace gloperate_qtquick
{


TextureItem::TextureItem(QQuickItem * parent)
: QQuickFramebufferObject(parent)
, m_path("")
{
}

TextureItem::~TextureItem()
{
}

const QString & TextureItem::path() const
{
    return m_path;
}

void TextureItem::setPath(const QString & path)
{
    m_path = path;
}

QQuickFramebufferObject::Renderer * TextureItem::createRenderer() const
{
    // Get gloperate environment
    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());
    gloperate::Environment * environment = engine->environment();

    // Create renderer
    return new TextureItemRenderer(const_cast<TextureItem *>(this), environment);
}


} // namespace gloperate_qtquick
