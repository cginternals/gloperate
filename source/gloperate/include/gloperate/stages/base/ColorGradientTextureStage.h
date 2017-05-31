
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/base/ColorGradientList.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


class ColorGradientList;


/**
*  @brief
*    Stage that maintains a texture containing gradients
*/
class GLOPERATE_API ColorGradientTextureStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientTextureStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that maintains a texture containing gradients"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    Input<ColorGradientList *>  gradients;       ///< List of gradients
    Input<size_t>               textureWidth;    ///< Size of texture in x dimension

    Output<globjects::Texture*> gradientTexture; ///< Texture with gradients


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


protected:
    /**
    *  @brief
    *    Process one iteration
    *
    *  @param[in] context
    *    Pointer to OpenGL context (must not be 'nullptr')
    *
    *  @remarks
    *    Overriden
    */
    virtual void onProcess() override;


protected:
    // Data
    std::unique_ptr<globjects::Texture> m_gradientTexture;  ///< Gradient texture
};


} // namespace gloperate
