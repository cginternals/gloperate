
#include "Postprocessing.h"

#include <glm/gtc/constants.hpp>

#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/base/ExplicitEquidistantColorGradient.h>
#include <gloperate/tools/ColorGradientPreparation.h>


Postprocessing::Postprocessing(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: PipelinePainter("Postprocessing", resourceManager, moduleInfo, m_pipeline)
, m_pipeline(moduleInfo)
, m_animation(true)
{
    // Setup painter
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

    gloperate::ColorGradientList & gradients = m_pipeline.gradients.data();
    gradients.add(new gloperate::ExplicitEquidistantColorGradient("Sequential Single-Hue Discrete", true,
        reflectionzeug::Color(239,243,255),
        reflectionzeug::Color(189,215,231),
        reflectionzeug::Color(107,174,214),
        reflectionzeug::Color(49,130,189),
        reflectionzeug::Color(8,81,156)
    ));
    gradients.add(new gloperate::ExplicitEquidistantColorGradient("Sequential Multi-Hue Interpolated", false,
        reflectionzeug::Color(255,255,204),
        reflectionzeug::Color(194,230,153),
        reflectionzeug::Color(120,198,121),
        reflectionzeug::Color(49,163,84),
        reflectionzeug::Color(0,104,55)
    ));
    gradients.add(new gloperate::ExplicitEquidistantColorGradient("Diverging Discrete", true,
        reflectionzeug::Color(202,0,32),
        reflectionzeug::Color(244,165,130),
        reflectionzeug::Color(247,247,247),
        reflectionzeug::Color(146,197,222),
        reflectionzeug::Color(5,113,176)
    ));
    gradients.add(new gloperate::ExplicitEquidistantColorGradient("Diverging Interpolated", false,
        reflectionzeug::Color(202,0,32),
        reflectionzeug::Color(244,165,130),
        reflectionzeug::Color(247,247,247),
        reflectionzeug::Color(146,197,222),
        reflectionzeug::Color(5,113,176)
    ));
    gradients.add(new gloperate::ExplicitEquidistantColorGradient("Qualitative", true,
        reflectionzeug::Color(251,180,174),
        reflectionzeug::Color(179,205,227),
        reflectionzeug::Color(204,235,197),
        reflectionzeug::Color(222,203,228),
        reflectionzeug::Color(254,217,166),
        reflectionzeug::Color(255,255,204),
        reflectionzeug::Color(229,216,189),
        reflectionzeug::Color(253,218,236),
        reflectionzeug::Color(242,242,242)
    ));

    // Register properties
    addProperty<bool>("Animation", this, &Postprocessing::animation, &Postprocessing::setAnimation);
    reflectionzeug::Property<std::string> * backgroundProperty = createProperty("Background", m_pipeline.gradientName);

    gloperate::ColorGradientPreparation gradientsTool(gradients, { 80, 20 });

    gradientsTool.configureProperty(backgroundProperty);
    backgroundProperty->setValue(gradientsTool.names().front());

    addProperty(backgroundProperty);
}

bool Postprocessing::animation() const
{
    return m_animation;
}

void Postprocessing::setAnimation(const bool & enabled)
{
    m_animation = enabled;

    gloperate::VirtualTimeCapability * timeCapability = getCapability<gloperate::VirtualTimeCapability>();
    if (timeCapability) {
        timeCapability->setEnabled(m_animation);
    }
}
