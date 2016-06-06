
#include <gloperate/stages/base/ProceduralTextureStage.h>

#include <random>

#include <glbinding/gl/gl.h>

#include <gloperate/gloperate-version.h>


namespace gloperate
{


ProceduralTextureStage::ProceduralTextureStage(ViewerContext * viewerContext, const std::string & name)
: Stage(viewerContext, name)
, texture("texture", this, nullptr)
{
}

ProceduralTextureStage::~ProceduralTextureStage()
{
}

void ProceduralTextureStage::onContextInit(AbstractGLContext *)
{
}

void ProceduralTextureStage::onContextDeinit(AbstractGLContext *)
{
}

void ProceduralTextureStage::onProcess(AbstractGLContext *)
{
    // Check if texture needs to be rebuilt
    if (!texture.isValid())
    {
        // Create texture
        createTexture();

        // Update outputs
        texture.setValue(m_texture.get());
    }
}

void ProceduralTextureStage::createTexture()
{
    static const int w(256);
    static const int h(256);
    unsigned char data[w * h * 4];

    std::random_device rd;
    std::mt19937 generator(rd());
    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < w * h * 4; ++i) {
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));
    }

    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_texture->image2D(0, gl::GL_RGBA8, w, h, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);
}


CPPEXPOSE_COMPONENT(
    ProceduralTextureStage, gloperate::Stage
  , ""   // Tags
  , ""   // Icon
  , ""   // Annotations
  , "Stage that creates a procedural texture"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate
