
#pragma once


#include <cppexpose/plugin/plugin_api.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <glm/glm.hpp>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Program;
    class State;
}


namespace gloperate
{


class AbstractDrawable;
class Camera;
class RenderPass;


/**
*  @brief
*    Stage that creates a render pass
*
*    Additional inputs of the following types can be created dynamically:
*
*    Type                   Description
*    ------------------     -----------
*    globjects::Texture  -  textures are attached via their input name
*    globjects::Buffer   -  buffers are added as shader storage buffers
*    uniforms of type T  -  other types are added as uniforms via their input name
*/
class GLOPERATE_API RenderPassStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        RenderPassStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a render pass"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gloperate::AbstractDrawable *> drawable;    ///< The drawable to be drawn
    Input<globjects::Program *>          program;     ///< The program used for rendering
    Input<gloperate::Camera *>           camera;      ///< The input camera
    Input<glm::mat4>                     modelMatrix; ///< Transformation matrix

    Input<bool>       depthTest; ///< Enable depth test?
    Input<bool>       depthMask; ///< Enable writing to the depth buffer?
    Input<gl::GLenum> depthFunc; ///< Depth function (e.g., gl::GL_LEQUAL)
    Input<bool>       culling;   ///< Enable culling?
    Input<gl::GLenum> cullFace;  ///< Cull face (e.g., gl::GL_BACK or gl::GL_FRONT)
    Input<gl::GLenum> frontFace; ///< Front face (e.g., gl::GL_CCW)
    Input<bool>       blending;  ///< Enable blending?

    // Additional dynamic inputs can be created, see class description

    // Outputs
    Output<gloperate::AbstractDrawable *> renderPass; ///< The created and configured RenderPass


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    RenderPassStage(Environment * environment, const std::string & name = "RenderPassStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderPassStage();

    /**
    *  @brief
    *    createNewUniformInput
    *
    *  @tparam T
    *    Type of the uniform
    *  @param[in] name
    *    Name of the new uniform
    *  @param[in] defaultValue
    *    The default value of the uniform, equals to default constructor
    */
    template <typename T>
    Input<T> & createNewUniformInput(const std::string & name, const T & defaultValue = T());


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;

    // Helper functions
    void setUniformValue(globjects::Program * program, AbstractSlot * input);


protected:
    // OpenGL objects
    std::unique_ptr<gloperate::RenderPass> m_renderPass;  ///< The created render pass
    std::unique_ptr<globjects::State>      m_beforeState; ///< OpenGL states for rendering

    // Signal connections
    cppexpose::ScopedConnection m_inputAddedConnection;
    cppexpose::ScopedConnection m_inputRemovedConnection;
};


} // namespace gloperate
