
#pragma once


#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


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
    */
    RenderItemRenderer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItemRenderer();

    // Virtual QQuickFramebufferObject::Renderer interface
    QOpenGLFramebufferObject * createFramebufferObject(const QSize & size) Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
    void synchronize(QQuickFramebufferObject * item) Q_DECL_OVERRIDE;
};


} // namespace gloperate_qtquick
