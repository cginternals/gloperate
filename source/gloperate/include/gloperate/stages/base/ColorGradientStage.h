
#pragma once


#include <cppassist/fs/FilePath.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


class ColorGradientList;


/**
*  @brief
*    Stage that loads a color gradient list from a file path
*/
class GLOPERATE_API ColorGradientStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that loads a color gradient list from a file path"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<cppassist::FilePath>  filePath;  ///< Path to the source file of the color gradient list

    // Outputs
    Output<ColorGradientList *> gradients; ///< List of color gradients
    Output<unsigned int>        size;      ///< Number of loaded color gradients


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
    ColorGradientStage(Environment * environment, const std::string & name = "ColorGradientStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorGradientStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<ColorGradientList> m_colorGradientList; ///< Shader object
};


} // namespace gloperate
