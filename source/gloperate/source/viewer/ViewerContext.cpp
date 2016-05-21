
#include <gloperate/viewer/ViewerContext.h>

#include <algorithm>

#include <gloperate/viewer/Surface.h>
#include <gloperate/scripting/ScriptEnvironment.h>

// Local components
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/demos/DemoPipeline.h>
#include <gloperate/stages/demos/DemoStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>


namespace gloperate
{


ViewerContext::ViewerContext()
: m_timeManager(this)
, m_inputManager()
, m_scriptEnvironment(this)
, m_resourceManager(this)
{
    registerLocalPlugins();
}

ViewerContext::~ViewerContext()
{
}

ViewerContext::ViewerContext(ViewerContext && other)
: m_timeManager(std::move(other.m_timeManager))
, m_surfaces(std::move(other.m_surfaces))
, m_inputManager(std::move(other.m_inputManager))
, m_scriptEnvironment(std::move(other.m_scriptEnvironment))
, m_componentManager(std::move(other.m_componentManager))
, m_resourceManager(std::move(other.m_resourceManager))
{
}

const TimeManager * ViewerContext::timeManager() const
{
    return &m_timeManager;
}

TimeManager * ViewerContext::timeManager()
{
    return &m_timeManager;
}

const InputManager * ViewerContext::inputManager() const
{
    return &m_inputManager;
}

InputManager * ViewerContext::inputManager()
{
    return &m_inputManager;
}

const ScriptEnvironment * ViewerContext::scriptEnvironment() const
{
    return &m_scriptEnvironment;
}

ScriptEnvironment * ViewerContext::scriptEnvironment()
{
    return &m_scriptEnvironment;
}

const cppexpose::ComponentManager * ViewerContext::componentManager() const
{
    return &m_componentManager;
}

cppexpose::ComponentManager * ViewerContext::componentManager()
{
    return &m_componentManager;
}

const ResourceManager * ViewerContext::resourceManager() const
{
    return &m_resourceManager;
}

ResourceManager * ViewerContext::resourceManager()
{
    return &m_resourceManager;
}

void ViewerContext::registerLocalPlugins()
{
    m_componentManager.addComponent(&BasicFramebufferStage::Component);
    m_componentManager.addComponent(&TextureLoadStage::Component);
    m_componentManager.addComponent(&ProceduralTextureStage::Component);
    m_componentManager.addComponent(&MixerStage::Component);
    m_componentManager.addComponent(&SplitStage::Component);
    m_componentManager.addComponent(&DemoPipeline::Component);
    m_componentManager.addComponent(&DemoStage::Component);
    m_componentManager.addComponent(&DemoTriangleStage::Component);
    m_componentManager.addComponent(&DemoTimerStage::Component);
}

void ViewerContext::registerSurface(Surface * surface)
{
    m_surfaces.push_back(surface);
}

void ViewerContext::unregisterSurface(Surface * surface)
{
    m_surfaces.erase(std::find(m_surfaces.begin(), m_surfaces.end(), surface));
}

const std::vector<Surface *> & ViewerContext::surfaces() const
{
    return m_surfaces;
}

std::vector<Surface *> & ViewerContext::surfaces()
{
    return m_surfaces;
}

bool ViewerContext::update()
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update();

    // Update surface
    for (Surface * surface : m_surfaces)
    {
        surface->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

bool ViewerContext::update(float delta)
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update(delta);

    // Update surface
    for (Surface * surface : m_surfaces)
    {
        surface->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

void ViewerContext::exit(int exitCode)
{
    // Emit signal
    this->exitApplication(exitCode);
}

ViewerContext & ViewerContext::operator=(ViewerContext && other)
{
    m_timeManager = std::move(other.m_timeManager);
    m_surfaces = std::move(other.m_surfaces);
    m_inputManager = std::move(other.m_inputManager);
    m_scriptEnvironment = std::move(other.m_scriptEnvironment);
    m_componentManager = std::move(other.m_componentManager);
    m_resourceManager = std::move(other.m_resourceManager);

    return *this;
}


} // namespace gloperate
