
#include <gloperate/pipeline/PipelineSaver.h>

#include <fstream>

#include <cppassist/logging/logging.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate{

PipelineSaver::PipelineSaver(const Stage *toSave)
: m_toSave{toSave}
{

}

void PipelineSaver::save(const std::string &filename)
{
    std::ofstream file{filename};

    if (!file.is_open())
    {
        cppassist::debug("Error opening file: " + filename);
        return;
    }

    auto writer = [&file](const std::string& toWrite, uint level)
    {
        const std::string padString{"    "};
        for(uint i = 0; i < level; i++)
        {
            file << padString;
        }

        file << toWrite << "\n";
    };

    saveStage(m_toSave, writer, 0u);

    file.close();

}

void PipelineSaver::saveStage(const Stage *stage, std::function<void(const std::string&, uint)> writer, uint level) const
{
    writer(stage->className() + " " + stage->name(), level);
    writer("{", level);
    for(const auto& input : stage->inputs())
    {

        std::string description = input->isDynamic() ? std::string{"input " + input->typeName() + " " + input->name()} : input->name();

        std::string value = "";

        if(input->isConnected())
        {
            value = getSourcePath(input);
        }
        else
        {
            value = input->toVariant().toJSON();
            if(input->type() == typeid(std::string)){
                value = '"' + value + '"';
            }
        }

        writer(description + ": " + value, level+1);
    }

    for(const auto& output : stage->outputs())
    {
        if(!output->isDynamic())
        {
            continue;
        }

        std::string description = "output " + output->typeName() + " " + output->name();
        writer(description , level+1);
    }

    if(stage->isPipeline())
    {
        auto pipeline = dynamic_cast<const Pipeline *>(stage);
        for( const auto subStage : pipeline->stages())
        {
            writer("", level);
            saveStage(subStage, writer, level+1);
        }
    }

    writer("}", level);
}

std::string PipelineSaver::getSourcePath(const AbstractSlot *slot) const
{
    if(!slot->isConnected()){
        return "unconnected";
    }

    std::stringstream path;

    path << slot->parent()->relativePathTo(slot->source()->parent());

    path << "." << slot->source()->name();

    return path.str();
}

}
