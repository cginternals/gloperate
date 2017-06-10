
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


enum class ShapeType : int;

class AbstractDrawable;
class Shape;


/**
*  @brief
*    Stage that creates a shape geometry
*/
class GLOPERATE_API ShapeStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShapeStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Stage that creates a shape geometry"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gloperate::ShapeType> shapeType; ///< Type of shape to create
    Input<float>                width;     ///< Width of the shape
    Input<float>                height;    ///< Height of the shape
    Input<float>                depth;     ///< Depth of the shape
    Input<float>                radius;    ///< Depth of the shape
    Input<bool>                 texCoords; ///< If 'true', texture coordinates are generated, else 'false'

    // Outputs
    Output<gloperate::AbstractDrawable *> drawable; ///< Shape drawable


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
    ShapeStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShapeStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<gloperate::Shape> m_shape; ///< The generated shape
};


} // namespace gloperate
