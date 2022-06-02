
#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <gloperate/input/constants.h>

#include <gloperate-headless/Surface.h>


namespace gloperate
{
    class Environment;
    class Canvas;
}


namespace gloperate_headless
{


/**
*  @brief
*    Default rendering surface for gloperate scenes
*/
class GLOPERATE_HEADLESS_API RenderSurface : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the window belongs (must NOT be null)
    */
    RenderSurface(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderSurface();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment (cannot be null)
    */
    const gloperate::Environment * environment() const;

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment (cannot be null)
    */
    gloperate::Environment * environment();

    /**
    *  @brief
    *    Get canvas
    *
    *  @return
    *    Canvas that is rendered on
    */
    gloperate::Canvas * canvas() const;


protected:
    // Virtual Window functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;


protected:
    gloperate::Environment           * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null) 
    std::unique_ptr<gloperate::Canvas> m_canvas;      ///< Canvas that controls the rendering onto the window (must NOT be null)
    glm::ivec2                         m_deviceSize;  ///< Window size (real device pixels)
    glm::ivec2                         m_virtualSize; ///< Window size (virtual pixel size)
};


} // namespace gloperate_headless
