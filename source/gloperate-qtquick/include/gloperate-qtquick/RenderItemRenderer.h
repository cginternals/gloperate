
#pragma once


#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class RenderItem;


/**
*  @brief
*    Renderer that executes the rendering into the FBO
*/
class GLOPERATE_QTQUICK_API RenderItemRenderer : public QQuickFramebufferObject::Renderer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] renderItem
    *    RenderItem into which is rendered (must NOT be null!)
    */
    RenderItemRenderer(RenderItem * renderItem);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItemRenderer();

    // Virtual QQuickFramebufferObject::Renderer interface
    QOpenGLFramebufferObject * createFramebufferObject(const QSize & size) Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
    void synchronize(QQuickFramebufferObject * item) Q_DECL_OVERRIDE;


protected:
    RenderItem * m_renderItem; ///< RenderItem into which is rendered (never null)
    bool m_first;
};


} // namespace gloperate_qtquick
