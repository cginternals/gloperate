
#pragma once


#include <cppassist/fs/FilePath.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>


namespace gloperate
{
    class AbstractDrawable;
    class Drawable;
}


/**
*  @brief
*    Stage for loading geometry from file
*/
class GeometryImporterStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        GeometryImporterStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Stage for loading geometry from file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<cppassist::FilePath> filePath;  ///< path to file containing geometry

    // Outputs
    Output<gloperate::AbstractDrawable *> geometry; ///< the geometry


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
    GeometryImporterStage(gloperate::Environment * environment, const std::string & name = "GeometryImporterStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GeometryImporterStage();


protected:
    // Virtual Stage inteface
    virtual void onProcess() override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;


protected:
    std::unique_ptr<gloperate::Drawable> m_geometry; ///< geometry
};
