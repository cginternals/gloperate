
#include <gloperate-qtquick/Utils.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/Stage.h>

    
namespace gloperate_qtquick
{


void Utils::initContext()
{
    glbinding::Binding::initialize(false);
    globjects::init();
#ifdef DEBUG
    globjects::DebugMessage::enable(true);
#endif
}

void Utils::registerGlobjectsContext()
{
    globjects::registerCurrentContext();
}

void Utils::clearScreen(float red, float green, float blue, float alpha, bool clearDepthBuffer)
{
    gl::glClearColor(red, green, blue, alpha);

    if (clearDepthBuffer) gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    else                  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}

std::unique_ptr<gloperate::Stage> Utils::createRenderStage(gloperate::Environment * environment, const std::string & name)
{
    auto component = environment->componentManager()->component<gloperate::Stage>(name);
    if (component) {
        return component->createInstance(environment);
    }

    return cppassist::make_unique<gloperate::Stage>(environment);
}


std::unique_ptr<gloperate::AbstractCanvas> Utils::createCanvas(gloperate::Environment * environment, std::unique_ptr<gloperate::Stage> && renderStage)
{
    auto canvas = cppassist::make_unique<gloperate::Canvas>(environment);
    canvas->setRenderStage(std::move(renderStage));
    return std::move(canvas);
}


} // namespace gloperate_qtquick
