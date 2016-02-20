
#pragma once


#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


/**
*  @brief
*    Utility function for qt quick and OpenGL integration
*/
class GLOPERATE_QTQUICK_API Utils
{
public:
    /**
    *  @brief
    *    Initialize glbinding and globjects in the current context
    */
    static void initContext();
};


} // namespace gloperate_qtquick
