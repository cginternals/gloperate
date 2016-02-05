
#pragma once


#include <gloperate/base/OpenGLContext.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;

namespace gloperate
{
    class ContextFormat;
}


namespace gloperate_qt
{


class OpenGLWindow;


/**
*  @brief
*    OpenGL context implementation based on GLFW
*/
class GLOPERATE_QT_API Context : public gloperate::OpenGLContext
{
public:
    Context(OpenGLWindow * window, QOpenGLContext * context);
    virtual ~Context();

    virtual glbinding::ContextHandle handle() const override;

    virtual const gloperate::ContextFormat & format() const override;

    virtual void use() const override;
    virtual void release() const override;


protected:
    Context();


protected:
    OpenGLWindow           * m_window;
    QOpenGLContext         * m_context;
    glbinding::ContextHandle m_handle;

    mutable gloperate::ContextFormat * m_format;


private:
    static void makeCurrent(QOpenGLContext * context, OpenGLWindow * window);
    static void doneCurrent(QOpenGLContext * context);
};


} // namespace gloperate_qt
