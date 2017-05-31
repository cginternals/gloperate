
#pragma once


#include <cppexpose/plugin/plugin_api.h>
#include <cppexpose/signal/ScopedConnection.h>

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
*        Type            -               Description
*    globjects::Texture  -  all textures are attached via their (input) name
*    globjects::Buffer   -  buffers are added as shader storage buffers
*    uniforms of type T  -  are updated automatically, see createNewUniformInput()
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
    Input<gloperate::AbstractDrawable *> drawable; ///< the drawable to be drawn
    Input<globjects::Program *>          program;  ///< the program used for rendering
    Input<gloperate::Camera *>           camera;   ///< the input camera

    // Additional dynamic inputs can be created, see class description

    // Outputs
    Output<gloperate::RenderPass *> renderPass; ///< The created and configured RenderPass


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
    void onContextInit(AbstractGLContext * content) override;

protected:
    std::unique_ptr<gloperate::RenderPass> m_renderPass;
    std::unique_ptr<globjects::State> m_beforeState;
    cppexpose::ScopedConnection m_inputAddedConnection;
    cppexpose::ScopedConnection m_inputRemovedConnection;

    std::unordered_map<std::string, std::function<void()>> uniformSetters; ///< Stores a lambda expression which updates the uniform value
};


} // namespace gloperate


#include <gloperate/stages/base/RenderPassStage.inl>
