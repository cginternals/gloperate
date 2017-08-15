
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class ColorGradientList;


/**
*  @brief
*    Stage that creates a texture containing color gradients
*/
class GLOPERATE_API ColorGradientTextureStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientTextureStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a texture containing color gradients"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<ColorGradientList *>   gradients;    ///< List of color gradients
    Input<size_t>                textureWidth; ///< Size of texture in X-dimension

    // Outputs
    Output<globjects::Texture *> texture;      ///< Texture with gradients


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
    ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorGradientTextureStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<globjects::Texture> m_gradientTexture; ///< Gradient texture
};


} // namespace gloperate
