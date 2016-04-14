
#pragma once


#include <string>
#include <functional>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class RenderSurface;


/**
*  @brief
*/
class GLOPERATE_API AbstractVideoTool
{
public:

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractVideoTool();

    /**
    *  @brief
    */
    virtual void init(const std::string & filename, gloperate::RenderSurface * surface, uint fps, uint length, uint width, uint height) = 0;

    /**
    *  @brief
    */
    virtual void createVideo(std::function<void(int, int)> progress, bool glContextActive) = 0;
};


} // namespace gloperate
