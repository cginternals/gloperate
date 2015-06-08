
#include "Postprocessing.h"

#include <glm/gtc/constants.hpp>

#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>


Postprocessing::Postprocessing(gloperate::ResourceManager & resourceManager)
:   PipelinePainter(resourceManager, m_pipeline)
{
    auto targetFBO = addCapability(new gloperate::TargetFramebufferCapability());
    auto viewport = addCapability(new gloperate::ViewportCapability());
    auto time = addCapability(new gloperate::VirtualTimeCapability());
    auto camera = addCapability(new gloperate::CameraCapability());
    auto projection = addCapability(new gloperate::PerspectiveProjectionCapability(viewport));
    auto renderTargets = addCapability(new gloperate::TypedRenderTargetCapability());

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
