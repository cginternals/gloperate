
#include <gloperate/stages/multiframe/TransparencyKernelStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/TransparencyMasksGenerator.h>


namespace gloperate
{


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, reprocess("reprocess", this)
, transparencyMaskTexture("transparencyMaskTexture", this)
{
}

void TransparencyKernelStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    const auto table = TransparencyMasksGenerator::generateDistributions(1);

    (*transparencyMaskTexture) = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    (*transparencyMaskTexture)->image2D(0, gl::GL_R8, table->at(0).size(), table->size(), 0, gl::GL_RED, gl::GL_UNSIGNED_BYTE, table->data());
}

void TransparencyKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    // invalidateOutputs();
}


} // namespace gloperate
