
#pragma once


#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>


namespace cppexpose
{
    class Variant;
}


namespace gloperate
{


class ViewerContext;
class Stage;


/**
*  @brief
*    Pipeline API for accessing the rendering pipeline
*/
class GLOPERATE_API PipelineApi : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    PipelineApi(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~PipelineApi();


protected:
    // Scripting functions
    std::string getName(const std::string & name);
    cppexpose::Variant getStages(const std::string & name);
    cppexpose::Variant getInputs(const std::string & name);
    cppexpose::Variant getOutputs(const std::string & name);

    // Helper functions
    Stage * getStage(const std::string & name);


protected:
    ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null!)
};


} // namespace gloperate
