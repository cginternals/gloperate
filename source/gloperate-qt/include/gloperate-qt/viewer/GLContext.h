
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QWindow;
class QOpenGLContext;


namespace gloperate_qt
{


/**
*  @brief
*    OpenGL context implementation based on Qt
*/
class GLOPERATE_QT_API GLContext : public gloperate::AbstractGLContext
{
    friend class GLContextFactory;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    Qt window that contains the context
    *  @param[in] context
    *    Qt OpenGL context
    *  @param[in] takeOwnership
    *    If 'true', the wrapper takes ownership over the Qt context, else 'false'
    */
    GLContext(QWindow * window, QOpenGLContext * context, bool takeOwnership = true);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContext();

    /**
    *  @brief
    *    Get Qt OpenGL context
    *
    *  @return
    *    Qt OpenGL context
    */
    QOpenGLContext * qtContext() const;

    // Virtual gloperate::AbstractGLContext functions
    virtual void use() const override;
    virtual void release() const override;


protected:
    QWindow        * m_window;      ///< Qt window that contains the context
    QOpenGLContext * m_context;     ///< Qt OpenGL context
    bool             m_ownsContext; ///< If 'true', the wrapper owns the Qt context, else 'false'


protected:
    void destroyContext();


private:
    static void makeCurrent(QOpenGLContext * context, QWindow * window);
    static void doneCurrent(QOpenGLContext * context);
    static void initGLBinding();
};


} // namespace gloperate_qt
