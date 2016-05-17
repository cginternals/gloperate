
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
    virtual void init(const std::string & filename, gloperate::RenderSurface * surface, uint width, uint height, uint fps, uint length) = 0;

    /**
    *  @brief
    */
    virtual void createVideo(std::function<void(int, int)> progress, bool glContextActive) = 0;
};


} // namespace gloperate
