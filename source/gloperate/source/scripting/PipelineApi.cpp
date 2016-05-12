
#include <gloperate/scripting/PipelineApi.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


PipelineApi::PipelineApi(ViewerContext * viewerContext)
: cppexpose::Object("pipeline")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("getName",    this, &PipelineApi::getName);
    addFunction("getInputs",  this, &PipelineApi::getInputs);
    addFunction("getOutputs", this, &PipelineApi::getOutputs);
}

PipelineApi::~PipelineApi()
{
}

std::string PipelineApi::getName()
{
    if (!m_viewerContext) return "";
    if (m_viewerContext->surfaces().size() == 0) return "";
    RenderSurface * surface = static_cast<RenderSurface *>(m_viewerContext->surfaces()[0]);
    Stage * stage = surface ? surface->renderStage() : nullptr;
    if (!stage) return "";

    return stage->name();
}

cppexpose::Variant PipelineApi::getInputs()
{
    if (!m_viewerContext) return cppexpose::Variant();
    if (m_viewerContext->surfaces().size() == 0) return cppexpose::Variant();
    RenderSurface * surface = static_cast<RenderSurface *>(m_viewerContext->surfaces()[0]);
    Stage * stage = surface ? surface->renderStage() : nullptr;
    if (!stage) return cppexpose::Variant();

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto * input : stage->inputs())
    {
        lst.asArray()->push_back(input->name());
    }
    return lst;
}

cppexpose::Variant PipelineApi::getOutputs()
{
    if (!m_viewerContext) return cppexpose::Variant();
    if (m_viewerContext->surfaces().size() == 0) return cppexpose::Variant();
    RenderSurface * surface = static_cast<RenderSurface *>(m_viewerContext->surfaces()[0]);
    Stage * stage = surface ? surface->renderStage() : nullptr;
    if (!stage) return cppexpose::Variant();

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto * output : stage->outputs())
    {
        lst.asArray()->push_back(output->name());
    }
    return lst;
}


} // namespace gloperate
