
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


class Environment;
class Stage;
class AbstractSlot;


// [TODO] Remove the entire class, access pipeline through cppexpose object hierarchy


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
    *  @param[in] environment
    *    Environment (must NOT be null!)
    */
    PipelineApi(Environment * environment);

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
    void setValue(const std::string & path, const std::string & value);
    bool isValid(const std::string & path);
    bool isRequired(const std::string & path);
    void setRequired(const std::string & path, bool required);

    // Helper functions
    Stage * getStage(const std::string & name);
    AbstractSlot * getSlot(const std::string & name);


protected:
    Environment * m_environment; ///< Gloperate environment (must NOT be null!)
};


} // namespace gloperate
