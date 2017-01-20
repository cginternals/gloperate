
#include <gloperate/stages/multiframe/TransparencyKernelStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/TransparencyMasksGenerator.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TransparencyKernelStage, gloperate::Stage)


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, reprocess("reprocess", this, true)
, transparencyMaskTexture("transparencyMaskTexture", this)
{
}

void TransparencyKernelStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    regenerateKernel();
}

void TransparencyKernelStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    if (*reprocess)
        regenerateKernel();
}

void TransparencyKernelStage::regenerateKernel()
{
    const auto table = TransparencyMasksGenerator::generateDistributions(1);
    m_texture->image2D(0, gl::GL_R8, table->at(0).size(), table->size(), 0, gl::GL_RED, gl::GL_UNSIGNED_BYTE, table->data());
    transparencyMaskTexture.setValue(m_texture);
}


} // namespace gloperate
