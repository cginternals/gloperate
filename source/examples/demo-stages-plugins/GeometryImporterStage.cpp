
#include "GeometryImporterStage.h"

#include <gloperate/base/Environment.h>
#include <gloperate/rendering/Drawable.h>


CPPEXPOSE_COMPONENT(GeometryImporterStage, gloperate::Stage)


GeometryImporterStage::GeometryImporterStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, filePath("filepath", this)
, geometry("geometry", this)
, m_geometry(nullptr)
{
}

GeometryImporterStage::~GeometryImporterStage()
{
}

void GeometryImporterStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_geometry.reset();
}

void GeometryImporterStage::onProcess()
{
    auto loaded = std::unique_ptr<gloperate::Drawable>(m_environment->resourceManager()->load<gloperate::Drawable>((*filePath).path()));

    if (loaded)
    {
        m_geometry = std::move(loaded);
        geometry = m_geometry.get();
    }
}
