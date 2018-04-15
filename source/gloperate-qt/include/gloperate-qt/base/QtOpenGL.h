
#pragma once


#include <glbinding/ProcAddress.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;


namespace gloperate_qt
{


/**
*  @brief
*    Helper functions to work with Qt and OpenGL
*/
class GLOPERATE_QT_API QtOpenGL
{
public:
    /**
    *  @brief
    *    Look up OpenGL function by name
    *
    *  @param[in] context
    *    Qt OpenGL context
    *  @param[in] name
    *    Function name
    *
    *  @return
    *    Function pointer
    */
    static glbinding::ProcAddress getProcAddress(QOpenGLContext * context, const char * name);
};


} // namespace gloperate_qt
