
#include "Postprocessing.h"

#include <glm/gtc/constants.hpp>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>


using gloperate::make_unique;

Postprocessing::Postprocessing(gloperate::ResourceManager & resourceManager)
:   PipelinePainter(resourceManager, m_pipeline)
{
    auto targetFBO = addCapability(make_unique<gloperate::TargetFramebufferCapability>());
    auto viewport = addCapability(make_unique<gloperate::ViewportCapability>());
    auto time = addCapability(make_unique<gloperate::VirtualTimeCapability>());
    auto camera = addCapability(make_unique<gloperate::CameraCapability>());
    auto projection = addCapability(make_unique<gloperate::PerspectiveProjectionCapability>(viewport));
    auto renderTargets = addCapability(make_unique<gloperate::TypedRenderTargetCapability>());

    m_pipeline.targetFBO.setData(targetFBO);
    m_pipeline.viewport.setData(viewport);
    m_pipeline.time.setData(time);
    m_pipeline.camera.setData(camera);
    m_pipeline.projection.setData(projection);
    m_pipeline.renderTargets.setData(renderTargets);

    targetFBO->changed.connect([this]() { m_pipeline.targetFBO.invalidate(); });
    viewport->changed.connect([this]() { m_pipeline.viewport.invalidate(); });
    time->changed.connect([this]() { m_pipeline.time.invalidate(); });
    
    projection->setZNear(0.1f);
    projection->setZFar(16.f);
    
    time->setLoopDuration(glm::pi<float>() * 2);
}
