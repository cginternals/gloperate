
#include <vector>
#include <fstream>

#include <cppassist/logging/logging.h>
#include <cppassist/cmdline/ArgumentParser.h>

#include <glbinding/gl/gl.h>
#include <globjects/Framebuffer.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate/tools/ImageExporter.h>

#include <gloperate-headless/Application.h>
#include <gloperate-headless/RenderSurface.h>
#include <gloperate-headless/GLContext.h>


using namespace gloperate;
using namespace gloperate_headless;


int main(int argc, char * argv[])
{
    // Read command line options
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);

    const auto contextString = argumentParser.value("--context");

    // Create gloperate environment
    Environment environment;

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins();

    // Initialize EGL
    Application::init();
    Application app(&environment, argc, argv);

    // Create render window
    RenderSurface surface(&app, &environment);

    const auto canvas = surface.canvas();

    // Specify desired context format
    gloperate::GLContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(gloperate::GLContextFormat::Profile::Core);
    format.setForwardCompatible(true);

    if (!contextString.empty())
    {
        if (!format.initializeFromString(contextString))
        {
            return 1;
        }
    }

    surface.setContextFormat(format);

    const auto width = 1280;
    const auto height = 720;

    canvas->loadRenderStage("ShapeDemo");
    surface.setTitle("gloperate headless viewer");
    surface.setSize(width, height);
    if (!surface.create())
    {
        return 1;
    }

    surface.repaint();

    // Initialize context, print context info
    surface.context()->use();
    cppassist::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Profile:  " << GLContextUtils::profile() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    surface.context()->release();

    // Assumes rendering to default framebuffer
    app.frame();

    surface.context()->use();

    std::vector<gl::GLubyte> pixels(width * height * 3 * sizeof(gl::GLubyte));
    const auto fbo = globjects::Framebuffer::defaultFBO();
    gl::glReadBuffer(gl::GL_BACK_LEFT);
    gl::glReadPixels(0, 0, width, height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, pixels.data());
    gl::glFinish();

    std::fstream stream("test.1280.720.rgb.ub.raw", std::fstream::out | std::fstream::binary);
    stream.write(reinterpret_cast<const char *>(pixels.data()), pixels.size());
    stream.close();

    surface.context()->release();

    return 0;
}
