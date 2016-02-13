
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;


namespace gloperate_qt
{


class OpenGLWindow;


/**
*  @brief
*    OpenGL context implementation based on Qt
*/
class GLOPERATE_QT_API GLContext : public gloperate::AbstractGLContext
{
public:
    GLContext(OpenGLWindow * window, QOpenGLContext * context);
    virtual ~GLContext();

    virtual void use() const override;
    virtual void release() const override;


protected:
    OpenGLWindow   * m_window;
    QOpenGLContext * m_context;


private:
    static void makeCurrent(QOpenGLContext * context, OpenGLWindow * window);
    static void doneCurrent(QOpenGLContext * context);
};


} // namespace gloperate_qt
