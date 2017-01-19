#include <gloperate/pipeline/PipelineLoader.h>

#include <iostream>
#include <regex>

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
        m_componentsByType.insert({stage->name(), stage}); // TODO: this could be std::transform
    }
}

std::unique_ptr<Stage> PipelineLoader::load(const std::string & filename)
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
    return std::unique_ptr<Stage>{pipeline};
}

std::unique_ptr<Stage> PipelineLoader::parse(const std::string & document)
{
    // Set document
    m_tokenizer.setDocument(document);

    // Begin parsing
    auto pipeline = readDocument();
    return std::unique_ptr<Stage>{pipeline};
}

Stage* PipelineLoader::readDocument()
{
    // The first value in a document must be either an object or an array
    Tokenizer::Token token = m_tokenizer.parseToken();

    Stage* root = nullptr;

    if(m_componentsByType.find(token.content) != m_componentsByType.end()){

        auto component = m_componentsByType.find(token.content);

        root = (*component).second->createInstance(m_environment);
        readStage(root);
    }
    else
    {
        cppassist::critical()
            << "Expected a known StageType, got: "
            << token.content
            << std::endl;
    }

    resolvePaths();

    return root;
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

        // Test whether the element is a substage
        if(m_componentsByType.find(token.content) != m_componentsByType.end()){

            auto component = m_componentsByType.find(token.content);

            Pipeline * rootAsPipeline = dynamic_cast<Pipeline*>(root);

            if(rootAsPipeline == nullptr){
                cppassist::critical()
                    << root->name()
                    << " is not a pipeline but seems to contain stages"
                    << std::endl;
                return false;
            }

            Stage * stage = (*component).second->createInstance(m_environment);
            rootAsPipeline->addStage(stage);
            readStage(stage);
            continue;
        }

        AbstractSlot * slot = nullptr;

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

            token = m_tokenizer.parseToken();
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
                cppassist::critical()
                    << "Expected a valid slot name got: "
                    << token.content
                    << std::endl;
                continue;
            }

            Tokenizer::Token token = m_tokenizer.parseToken();

            // only read the value of the slot if it is expected
            if(token.content == ":"){
                bool slot_read = readSlot(slot);
                if(!slot_read)
                {
                    cppassist::critical()
                        << "Could not read the content for Slot "
                        << slot->name()
                        << std::endl;
                    continue;
                }
            }
        }

    }

    return true;

}

bool PipelineLoader::readSlot(gloperate::AbstractSlot *slot)
{
    Tokenizer::Token token = m_tokenizer.parseToken();

    // Read content of the slot
    // Todo: Manage connections

    return slot->fromVariant(cppexpose::Variant(token.content));
    // if it is a path: add to process later
    // idea: regex "((alnum+)(\.))+(alnum+)"
    // this does not catch connections to the same stage

    // check whether the token points to a path
    auto path_string = "([:alnum:]+\.)+[:alnum:]+";
    std::regex path_regex(path_string);
    if(std::regex_match(token.content, path_regex)) {
        auto pairing = std::make_pair(slot, token.content);
        m_unresolvedPaths.push_back(pairing);
        return true;
    }
    else {
        // if it is a value: read directly
        return slot->fromVariant(cppexpose::Variant(token.content));
    }
}

bool PipelineLoader::resolvePaths()
{
    for(auto& slot_and_path : m_unresolvedPaths)
    {
        AbstractSlot * slot = slot_and_path.first;
        std::string& path = slot_and_path.second;

        cppexpose::AbstractProperty * target = slot->parentStage()->property(path);

        AbstractSlot * targetSlot = dynamic_cast<AbstractSlot *>(target);

        if(targetSlot == nullptr)
        {
            // The target is no stage and therefore has no slots, so this is an error/wrong path

        }

        slot->connect(targetSlot);
    }

    return true;
}

} // namespace gloperate
