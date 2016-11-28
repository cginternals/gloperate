
#include <gloperate-glkernel/NoiseKernelStage.h>

#include <glkernel/sample.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(NoiseKernelStage, gloperate::Stage)


NoiseKernelStage::NoiseKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, dimensions("dimensions", this, glm::ivec3(1))
, kernel("kernel", this)
, texture("texture", this)
{
}


void NoiseKernelStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_texture = new globjects::Texture(gl::GL_TEXTURE_2D);
}


void NoiseKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    if (*dimensions != glm::ivec3(m_kernel.extent()))
    {
        resizeKernel();
    }

    regenerateKernel();

    m_texture->image3D(1, gl::GL_RGB32F, *dimensions, 0, gl::GL_RGB, gl::GL_FLOAT, m_kernel.data());

    kernel.setValue(m_kernel);
    texture.setValue(m_texture);
}


void NoiseKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel3(*dimensions);
}


void NoiseKernelStage::regenerateKernel()
{
    glkernel::sample::best_candidate(m_kernel);
}


}
