
#pragma once


#include <QSurfaceFormat>

#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QWindow;


namespace gloperate_qt
{


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_QT_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Convert context format description into Qt version
    *
    *  @param[in] format
    *    OpenGL context format description (gloperate version)
    *
    *  @return
    *    OpenGL context format description (Qt version)
    */
    static QSurfaceFormat toQSurfaceFormat(const gloperate::GLContextFormat & format);


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    Surface on which contexts are created (must NOT be null)
    */
    GLContextFactory(QWindow * window);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) override;


protected:
    QWindow * m_window; ///< Surface on which contexts are created
};


} // namespace gloperate_qt
