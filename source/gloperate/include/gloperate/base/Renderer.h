
#pragma once


#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
    class Program;
    class Shader;
    class AbstractStringSource;
}


namespace gloperate
{


class Environment;
class Camera;
class Box;

/**
*  @brief
*    Placeholder class to perform rendering, will be replaced by a pipeline
*/
class GLOPERATE_API Renderer : public cppexpose::Object
{
public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the renderer belongs (must NOT be null!)
    */
    Renderer(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Renderer();
    //@}

    //@{
    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment to which the renderer belongs (never null)
    */
    const Environment * environment() const;
    Environment * environment();
    //@}

    // Must be called from render thread
    void render(globjects::Framebuffer * targetFBO);

    virtual void onContextInit();
    virtual void onContextDeinit();

    // Must be called from render thread
    virtual void onRender(globjects::Framebuffer * targetFBO);

    // Must be called from UI thread
    // [TODO] Needed any more?
    virtual void onUpdate();

    // Must be called from UI thread
    virtual void onViewport(
        const glm::vec4 & deviceViewport
      , const glm::vec4 & virtualViewport);


protected:
    Environment       * m_environment;     ///< Gloperate environment to which the canvas belongs
    glm::vec4           m_deviceViewport;  ///< Viewport (in real device coordinates)
    glm::vec4           m_virtualViewport; ///< Viewport (in virtual coordinates)

    std::unique_ptr<globjects::Texture>              m_texture;
    std::unique_ptr<gloperate::Box>                  m_box;
    std::unique_ptr<globjects::Program>              m_program;
    std::unique_ptr<globjects::AbstractStringSource> m_vertexShaderSource;
    std::unique_ptr<globjects::AbstractStringSource> m_fragmentShaderSource;
    std::unique_ptr<globjects::Shader>               m_vertexShader;
    std::unique_ptr<globjects::Shader>               m_fragmentShader;
    std::unique_ptr<gloperate::Camera>               m_camera;
};


} // namespace gloperate
