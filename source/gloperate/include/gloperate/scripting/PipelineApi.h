
#pragma once


#include <vector>

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
class PipelineApiWatcher;


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
    cppexpose::Variant getParameters(const std::string & name);
    cppexpose::Variant getOutputs(const std::string & name);
    cppexpose::Variant getProxyOutputs(const std::string & name);
    std::string getValue(const std::string & path);
    void registerWatcher(const cppexpose::Variant & func);

    // Helper functions
    Stage * getStage(const std::string & name);
    cppexpose::AbstractProperty * getProperty(const std::string & name);


protected:
    ViewerContext                     * m_viewerContext; ///< Viewer context (must NOT be null!)
    std::vector<PipelineApiWatcher *>   m_watchers;      ///< List of watchers for pipeline events
};


} // namespace gloperate
