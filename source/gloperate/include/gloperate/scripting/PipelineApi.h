
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
    std::string getName();
    cppexpose::Variant getInputs();
    cppexpose::Variant getOutputs();


protected:
    ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null!)
};


} // namespace gloperate
