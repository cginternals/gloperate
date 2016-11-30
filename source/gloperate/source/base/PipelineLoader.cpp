#include <gloperate/base/PipelineLoader.h>

#include <iostream>

#include <cppassist/string/conversion.h>
#include <cppassist/logging/logging.h>


#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/AbstractSlot.h>

using namespace cppassist;

using cppexpose::Tokenizer;

namespace gloperate
{

std::unique_ptr<Pipeline> PipelineLoader::load(Environment * environment, const std::string & filename)
{
    // Create tokenizer for JSON
    Tokenizer tokenizer;
    tokenizer.setOptions(
        Tokenizer::OptionParseStrings
      | Tokenizer::OptionParseNumber
      | Tokenizer::OptionParseBoolean
      | Tokenizer::OptionParseNull
      | Tokenizer::OptionCStyleComments
      | Tokenizer::OptionCppStyleComments
    );
    tokenizer.setQuotationMarks("\"");
    tokenizer.setSingleCharacters("{}:");

    // Load file
    if (!tokenizer.loadDocument(filename))
    {
        return nullptr;
    }

    Pipeline * rootPipeline;

    // Begin parsing
    readDocument(rootPipeline, environment, tokenizer);
    return std::unique_ptr<Pipeline>{rootPipeline};
}

std::unique_ptr<Pipeline> PipelineLoader::parse(Environment * environment, const std::string & document)
{
    // Create tokenizer for JSON
    Tokenizer tokenizer;
    tokenizer.setOptions(
        Tokenizer::OptionParseStrings
      | Tokenizer::OptionParseNumber
      | Tokenizer::OptionParseBoolean
      | Tokenizer::OptionParseNull
      | Tokenizer::OptionCStyleComments
      | Tokenizer::OptionCppStyleComments
    );
    tokenizer.setQuotationMarks("\"");
    tokenizer.setSingleCharacters("{}");

    // Set document
    tokenizer.setDocument(document);

    Pipeline * rootPipeline;

    // Begin parsing
    readDocument(rootPipeline, environment, tokenizer);
    return std::unique_ptr<Pipeline>{rootPipeline};
}

bool PipelineLoader::readDocument(Pipeline * root, Environment* environment, Tokenizer & tokenizer)
{
    // The first value in a document must be either an object or an array
    Tokenizer::Token token = tokenizer.parseToken();

    if (token.type == Tokenizer::TokenWord && token.content == "pipeline")
    {
        root = new Pipeline(environment);
        readPipeline(root, environment, tokenizer);
    }

    else
    {
        cppassist::critical()
            << "A valid pipeline must begin with the keyword 'pipeline'. "
            << std::endl;

        return false;
    }

    return true;
}

bool PipelineLoader::readPipeline(Pipeline *root, Environment *environment, cppexpose::Tokenizer &tokenizer)
{
    Tokenizer::Token token = tokenizer.parseToken();

    if (token.type != Tokenizer::TokenWord){
        cppassist::critical()
            << "Expected name of the pipeline, got: "
            << token.content
            << std::endl;

        return false;
    }

    root->setName(token.content);

    token = tokenizer.parseToken();
    if (token.content != "{"){
        cppassist::critical()
            << "Expected '{' got: "
            << token.content
            << std::endl;

        return false;
    }

    while(token.content != "}")
    {
        token = tokenizer.parseToken();

        if(token.content == "pipeline"){
            Pipeline * pipeline = new Pipeline(environment);
            root->addStage(pipeline);
            readPipeline(pipeline, environment, tokenizer);
            continue;
        }
        if(token.content == "stage"){
            // TODO: resolve stagentype
            auto stageType = tokenizer.parseToken();
            Stage * stage = new Stage(environment);
            root->addStage(stage);
            readStage(stage, tokenizer);
            continue;
        }

        // TODO: Inputs/Outputs of Pipelines
    }

    return true;
}

bool PipelineLoader::readStage(Stage* root, cppexpose::Tokenizer &tokenizer)
{
    Tokenizer::Token token = tokenizer.parseToken();

    if (token.type != Tokenizer::TokenWord){
        cppassist::critical()
            << "Expected name of the stage, got: "
            << token.content
            << std::endl;

        return false;
    }

    root->setName(token.content);

    token = tokenizer.parseToken();
    if (token.content != "{"){
        cppassist::critical()
            << "Expected '{' got: "
            << token.content
            << std::endl;

        return false;
    }

    while(token.content != "}")
    {
        token = tokenizer.parseToken();

        // Test whether there is a dynamic property (input or output)
        if(token.content == "input" || token.content == "output"){
            auto slotTypeString = tokenizer.parseToken();
            // TODO: resolve slotType
            auto * slot = new Slot<slotType>();

            if(token.content == "input"){
                root->addInput(slot);
            }else{
                root->addInput(slot);
            }

            // A dynamic token is filled like a static one, so a fallthrough works just fine
            token = tokenizer.parseToken();
        }

        if(token.type == Tokenizer::TokenWord)
        {
            auto slot = root->input(token.content);
            if(slot == nullptr)
                slot = root->output(token.content);

            if(slot == nullptr)
            {
                return false;
                cppassist::critical()
                    << "Expected a valid slot name got: "
                    << token.content
                    << std::endl;
            }

            readSlot(root, slot, tokenizer);
        }

    }

    return true;

}

bool PipelineLoader::readSlot(Stage *root, gloperate::AbstractSlot *slot, cppexpose::Tokenizer &tokenizer)
{
    Tokenizer::Token token = tokenizer.parseToken();

    if(token.content != ":")
    {
        cppassist::critical()
            << "Expected ':' got: "
            << token.content
            << std::endl;

        return false;
    }

    if(token.content == "true" || token.content == "false")
        slot->setValue(cppexpose::Variant(token.content).toBool());

}

} // namespace gloperate
