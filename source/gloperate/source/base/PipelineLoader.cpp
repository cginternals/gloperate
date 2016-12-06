#include <gloperate/base/PipelineLoader.h>

#include <iostream>

#include <cppassist/string/conversion.h>
#include <cppassist/logging/logging.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/StageComponent.h>
#include <gloperate/pipeline/AbstractSlot.h>

#include <gloperate/base/Environment.h>

using namespace cppassist;

using cppexpose::Tokenizer;

using StageComponent = cppexpose::TypedComponent<gloperate::Stage>;

namespace gloperate
{

// TODO: build an error checked way to read a word

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



            if (token.type != Tokenizer::TokenWord){
                cppassist::critical()
                    << "Expected typename of a stage, got: "
                    << token.content
                    << std::endl;

                return false;
            }
            auto stageTypeName = token.content;

            // TODO: make this an instance variable, add a method "getComponentByTypename"
            std::vector<StageComponent* > stages = environment->componentManager()->components<gloperate::Stage>();

            auto component = std::find_if(stages.begin(),
                         stages.end(),
                         [&stageTypeName](StageComponent* curComponent){return curComponent->type() == stageTypeName;});

            if(component == stages.end()){
                cppassist::critical()
                    << "Expected the typename of a stage, but no stage of type: "
                    << token.content
                    << " seems to exist."
                    << std::endl;

                return false;
            }

            Stage * stage = (*component)->createInstance(environment);
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

        AbstractSlot * slot;

        // Test whether there is a dynamic property (input or output)
        if(token.content == "input" || token.content == "output"){

            auto slotType = token.content;

            token = tokenizer.parseToken();
            if (token.type != Tokenizer::TokenWord){
                cppassist::critical()
                    << "Expected the type of a slot got: "
                    << token.content
                    << std::endl;

                return false;
            }
            auto type = token.content;

            if (token.type != Tokenizer::TokenWord){
                cppassist::critical()
                    << "Expected the name of a slot got: "
                    << token.content
                    << std::endl;

                return false;
            }
            auto name = token.content;

            slot = root->createSlot(slotType, type, name);
            if(slot == nullptr){
                cppassist::critical()
                    << type
                    << " is not a valid type for a slot."
                    << std::endl;

                return false;
            }
            // A dynamic token is filled like a static one, so a fallthrough works just fine
        }

        if(token.type == Tokenizer::TokenWord)
        {
            // No need to search for a slot that was just created
            if(slot == nullptr)
                slot = root->input(token.content);
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

            readSlot(slot, tokenizer);
        }

    }

    return true;

}

bool PipelineLoader::readSlot(gloperate::AbstractSlot *slot, cppexpose::Tokenizer &tokenizer)
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

    return slot->fromVariant(cppexpose::Variant(token.content));
}

} // namespace gloperate
