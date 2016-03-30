
#include <gloperate-text/stages/GlyphRenderStage.h>

//#include <chrono>
//#include <iostream>
//#include <fstream>

#include <glbinding/gl/gl.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>

#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage()
{
    addInput("vertexCloud", vertexCloud);

    addInput("viewport", viewport);
    addInput("targetFramebuffer", targetFramebuffer);

    alwaysProcess(true);
}

GlyphRenderStage::~GlyphRenderStage()
{
}

void GlyphRenderStage::initialize()
{
    m_renderer.reset(new GlyphRenderer);
}

double avg = 0.0;
uint32_t n = 0;

void GlyphRenderStage::process()
{
    gl::glViewport(viewport.data()->x(), viewport.data()->y(), viewport.data()->width(), viewport.data()->height());

    globjects::Framebuffer * fbo = targetFramebuffer.data()->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }
    fbo->bind(gl::GL_FRAMEBUFFER);

    gl::glClearColor(1.f, 1.f, 1.f, 1.f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    gl::glDepthMask(gl::GL_FALSE);
    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    //gl::glFinish();
    //const auto t0 = std::chrono::high_resolution_clock::now();

    m_renderer->render(vertexCloud.data());
    
    //gl::glFinish();
    //const auto t1 = std::chrono::high_resolution_clock::now();

    //using nano = std::chrono::duration<double, std::micro>;

    //++n;
    //auto last = std::chrono::duration_cast<nano>(t1 - t0).count();
    //avg += last;

    //if (n == 1000)
    //{
    //    std::ofstream glyphlog;
    //    glyphlog.open("glyph.log", std::ios::out | std::ios::ate);
    //    glyphlog << "average:  " << avg / static_cast<double>(n) << "µs per frame (" << n << " frames)" << std::endl;
    //    glyphlog << "   last:  " << last << "µs" << std::endl;
    //    glyphlog.flush();
    //    glyphlog.close();

    //    avg = 0.0;
    //    n = 0;
    //}

    gl::glDepthMask(gl::GL_TRUE);
    gl::glDisable(gl::GL_CULL_FACE);
    gl::glDisable(gl::GL_BLEND);

    fbo->unbind(gl::GL_FRAMEBUFFER);
}


} // namespace gloperate_text
