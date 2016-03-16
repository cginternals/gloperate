
#include <gloperate-qtquick/viewer/Utils.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>

#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/stages/demos/DemoStage.h>

    
namespace gloperate_qtquick
{


void Utils::initContext()
{
    glbinding::Binding::initialize(false);
    globjects::init();
}

void Utils::clearScreen(float red, float green, float blue, float alpha, bool clearDepthBuffer)
{
    gl::glClearColor(red, green, blue, alpha);

    if (clearDepthBuffer) gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    else                  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}

gloperate::Stage * Utils::createRenderStage(gloperate::ViewerContext * viewerContext)
{
    return new gloperate::DemoStage(viewerContext);
}

gloperate::Surface * Utils::createSurface(gloperate::ViewerContext * viewerContext, gloperate::Stage * renderStage)
{
    gloperate::RenderSurface * renderSurface = new gloperate::RenderSurface(viewerContext);
    renderSurface->setRenderStage(renderStage);
    return renderSurface;
}


} // namespace gloperate_qtquick
