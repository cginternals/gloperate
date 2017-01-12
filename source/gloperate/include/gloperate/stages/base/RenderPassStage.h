
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that creates a render pass
*
*    Additional inputs of the following types can be created dynamically:
*    globjects::Texture - all textures are attached via their name
*    globjects::Buffer  - buffers are added as shader storage buffers
*    uniforms of type T - are updated automatically, see createNewUniformInput()
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
    Input<globjects::ref_ptr<gloperate::Drawable>> drawable;
    Input<globjects::ref_ptr<globjects::Program>> program;

    Input<gloperate::Camera *> camera;

    // Additional dynamic inputs can be created, see class description

    // Outputs
    Output<gloperate::RenderPass *> renderPass; ///< ToDo


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
    virtual void onProcess(AbstractGLContext * context) override;
    void onContextInit(AbstractGLContext * content) override;

protected:
    globjects::ref_ptr<gloperate::RenderPass> ///< RenderPass object

    std::unordered_map<std::string, std::function<void()>> uniformSetters; ///< ToDo
};


} // namespace gloperate
