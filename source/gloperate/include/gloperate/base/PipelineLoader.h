
#pragma once

#include <gloperate/base/PipelineLoader.h>
#include <cppexpose/base/Tokenizer.h>

#include <string>
#include <vector>
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
    static std::unique_ptr<Pipeline> load(gloperate::Environment *environment, const std::string & filename);

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
    static std::unique_ptr<Pipeline> parse(Environment *environment, const std::string & document);


private:
    static bool readDocument(gloperate::Pipeline* root, Environment* environment, cppexpose::Tokenizer& tokenizer);
    static bool readPipeline(gloperate::Pipeline* root, Environment* environment, cppexpose::Tokenizer& tokenizer);
    static bool readStage(gloperate::Stage* root, cppexpose::Tokenizer & tokenizer);
    static bool readSlot(gloperate::AbstractSlot * slot, cppexpose::Tokenizer& tokenizer);
};


} // namespace gloperate
