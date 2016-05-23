
#include <gloperate/scripting/PipelineApiWatcher.h>

#include <cppassist/logging/logging.h>

#include <gloperate/pipeline/PipelineEvent.h>
#include <gloperate/pipeline/AbstractSlot.h>


using namespace cppassist;


namespace gloperate
{


PipelineApiWatcher::PipelineApiWatcher(const cppexpose::Variant & func)
{
    info() << "PipelineApiWatcher created.";

    // Save callback function
    if (func.hasType<cppexpose::Function>()) {
        m_func = func.value<cppexpose::Function>();
    }
}

PipelineApiWatcher::~PipelineApiWatcher()
{
    info() << "PipelineApiWatcher destroyed.";
}

void PipelineApiWatcher::onPipelineEvent(const PipelineEvent & event)
{
    // [TODO]

    if (event.type() == PipelineEvent::ValueChanged)
    {
        info()
        << "ValueChanged: "
        << event.slot()->qualifiedName()
        << " ("
        << event.slot()->toString()
        << ")";
    }

    /*
    std::vector<cppexpose::Variant> params;
    m_func.call(params);
    */
}


} // namespace gloperate
