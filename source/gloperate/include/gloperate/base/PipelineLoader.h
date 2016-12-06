
#pragma once

#include <gloperate/base/PipelineLoader.h>
#include <cppexpose/base/Tokenizer.h>

#include <gloperate/pipeline/StageComponent.h>

#include <string>
#include <unordered_map>
#include <memory>


namespace gloperate
{
    class Pipeline;
    class Environment;
    class Stage;
    class AbstractSlot;
}


/**
*  @brief
*    JSON tools
*/

namespace gloperate{

class PipelineLoader
{
public:
    PipelineLoader(gloperate::Environment* environment);

    /**
    *  @brief
    *    Load pipeline from file
    *
    *  @param[in] filename
    *    Filename of pipeline file
    *
    *  @return
    *    returns the newly constructed pipeline
    *    returns nullptr when the file is inexistent or contains no valid pipeline
    */
    std::unique_ptr<Pipeline> load(const std::string & filename);

    /**
    *  @brief
    *    Parse pipeline from string
    *
    *  @param[in] document
    *    a string representation of the pipeline
    *
    *  @return
    *    returns the newly constructed pipeline
    *    returns nullptr when the file is inexistent or contains no valid pipeline
    */
    std::unique_ptr<Pipeline> parse(const std::string & document);


private:
    Pipeline *readDocument();
    bool readPipeline(gloperate::Pipeline* root);
    bool readStage(gloperate::Stage* root);
    bool readSlot(gloperate::AbstractSlot * slot);

    gloperate::Environment* m_environment;
    cppexpose::Tokenizer m_tokenizer;
    std::unordered_map<std::string, cppexpose::TypedComponent<gloperate::Stage> *> m_componentsByType;
};


} // namespace gloperate
