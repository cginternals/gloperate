
#pragma once

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
    std::unique_ptr<Stage> load(const std::string & filename);

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
    std::unique_ptr<Stage> parse(const std::string & document);


private:
    Stage *readDocument();
    bool readStage(Stage* root);
    bool readSlot(AbstractSlot * slot);
    bool resolvePaths();

    gloperate::Environment* m_environment;
    cppexpose::Tokenizer m_tokenizer;
    std::unordered_map<std::string, cppexpose::TypedComponent<gloperate::Stage> *> m_componentsByType;

    std::vector<std::pair<AbstractSlot *, std::string>> m_unresolvedPaths;
};


} // namespace gloperate
