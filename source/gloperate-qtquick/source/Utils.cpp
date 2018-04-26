
#include <gloperate-qtquick/Utils.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>


namespace gloperate_qtquick
{


void Utils::initContext(glbinding::GetProcAddress functionPointerResolver)
{
    // Initialize globjects and glbinding
    globjects::init(functionPointerResolver);
#ifdef DEBUG
    globjects::DebugMessage::enable(true);
#endif
}

void Utils::registerGlobjectsContext(glbinding::GetProcAddress functionPointerResolver)
{
    globjects::registerCurrentContext(functionPointerResolver);
}

void Utils::setGlobjectsContext()
{
    globjects::setCurrentContext();
}

void Utils::clearScreen(float red, float green, float blue, float alpha, bool clearDepthBuffer)
{
    gl::glClearColor(red, green, blue, alpha);

    if (clearDepthBuffer) gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    else                  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}


} // namespace gloperate_qtquick
