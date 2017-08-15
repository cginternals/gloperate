
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{



/**
*  @brief
*    Stage that selects a float value from a given index
*/
class GLOPERATE_API FloatSelectionStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        FloatSelectionStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that selects a float from a given index"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<unsigned int> index; ///< Index of float to select

    // Outputs
    Output<float>       value; ///< Resulting float value


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
    FloatSelectionStage(gloperate::Environment * environment, const std::string & name = "FloatSelectionStage");

    /**
    *  @brief
    *    Destructor
    */
    ~FloatSelectionStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;


protected:
    std::vector<Input<float> *> m_floatInputs; ///< The list of all connected float inputs
};


} // namespace gloperate
