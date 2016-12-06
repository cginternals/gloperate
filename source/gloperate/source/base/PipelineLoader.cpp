#include <gloperate/base/PipelineLoader.h>

#include <iostream>

#include <cppassist/string/conversion.h>
#include <cppassist/logging/logging.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/AbstractSlot.h>

#include <gloperate/base/Environment.h>

using namespace cppassist;

using cppexpose::Tokenizer;

using StageComponent = cppexpose::TypedComponent<gloperate::Stage>;

namespace gloperate
{

// TODO: build an error checked way to read a word

PipelineLoader::PipelineLoader(Environment *environment)
:   m_environment{environment}
{
    m_tokenizer.setOptions(
        Tokenizer::OptionParseStrings
      | Tokenizer::OptionParseNumber
      | Tokenizer::OptionParseBoolean
      | Tokenizer::OptionParseNull
      | Tokenizer::OptionCStyleComments
      | Tokenizer::OptionCppStyleComments
    );

    m_tokenizer.setQuotationMarks("\"");
    m_tokenizer.setSingleCharacters("{}:");

    std::vector<StageComponent* > stages = m_environment->componentManager()->components<gloperate::Stage>();

    for(auto stage : stages){
        m_componentsByType.insert({stage->type(), stage});
    }
}

std::unique_ptr<Pipeline> PipelineLoader::load(const std::string & filename)
{
    // Load file
    if (!m_tokenizer.loadDocument(filename))
    {
        cppassist::critical()
            << "Could not read the file: "
            << filename
            << ". Check that it exists and is accessible to the application."
            << std::endl;

        return nullptr;
    }

    // Begin parsing
    auto pipeline = readDocument();
    return std::unique_ptr<Pipeline>{pipeline};
}

std::unique_ptr<Pipeline> PipelineLoader::parse(const std::string & document)
{
    // Set document
    m_tokenizer.setDocument(document);

    // Begin parsing
    auto pipeline = readDocument();
    return std::unique_ptr<Pipeline>{pipeline};
}

Pipeline* PipelineLoader::readDocument()
{
    // The first value in a document must be either an object or an array
    Tokenizer::Token token = m_tokenizer.parseToken();

    Pipeline* root = nullptr;

    if (token.type == Tokenizer::TokenWord && token.content == "pipeline")
    {
        root = new Pipeline(m_environment);
        readPipeline(root);
    }
    else
    {
        cppassist::critical()
            << "A valid pipeline must begin with the keyword 'pipeline'. "
            << std::endl;
    }

    return root;
}

bool PipelineLoader::readPipeline(Pipeline *root)
{
    Tokenizer::Token token = m_tokenizer.parseToken();

    if (token.type != Tokenizer::TokenWord){
        cppassist::critical()
            << "Expected name of the pipeline, got: "
            << token.content
            << std::endl;

        return false;
    }

    root->setName(token.content);

    token = m_tokenizer.parseToken();
    if (token.content != "{"){
        cppassist::critical()
            << "Expected '{' got: "
            << token.content
            << std::endl;

        return false;
    }

    while(token.content != "}")
    {
        token = m_tokenizer.parseToken();

        if(token.content == "pipeline"){
            Pipeline * pipeline = new Pipeline(m_environment);
            root->addStage(pipeline);
            readPipeline(pipeline);
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

            auto component = m_componentsByType.find(stageTypeName);

            if(component == m_componentsByType.end()){
                cppassist::critical()
                    << "Expected the typename of a stage, but no stage of type: "
                    << token.content
                    << " seems to exist."
                    << std::endl;

                return false;
            }

            Stage * stage = (*component).second->createInstance(m_environment);
            root->addStage(stage);
            readStage(stage);
            continue;
        }

        // TODO: Inputs/Outputs of Pipelines
    }

    return true;
}


bool PipelineLoader::readStage(Stage* root)
{
    Tokenizer::Token token = m_tokenizer.parseToken();

    if (token.type != Tokenizer::TokenWord){
        cppassist::critical()
            << "Expected name of the stage, got: "
            << token.content
            << std::endl;

        return false;
    }

    root->setName(token.content);

    token = m_tokenizer.parseToken();
    if (token.content != "{"){
        cppassist::critical()
            << "Expected '{' got: "
            << token.content
            << std::endl;

        return false;
    }

    while(token.content != "}")
    {
        token = m_tokenizer.parseToken();

        AbstractSlot * slot;

        // Test whether there is a dynamic property (input or output)
        if(token.content == "input" || token.content == "output"){

            auto slotType = token.content;

            token = m_tokenizer.parseToken();
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

            readSlot(slot);
        }

    }

    return true;

}

bool PipelineLoader::readSlot(gloperate::AbstractSlot *slot)
{
    Tokenizer::Token token = m_tokenizer.parseToken();

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
