
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include <glkernel/noise.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(NoiseKernelStage, gloperate::Stage)


NoiseKernelStage::NoiseKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, dimensions("dimensions", this, glm::ivec3(1))
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
{
}

NoiseKernelStage::~NoiseKernelStage()
{
}

void NoiseKernelStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D_ARRAY);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_REPEAT);

    texture.invalidate();
}

void NoiseKernelStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_texture.reset();

    texture.setValue(nullptr);
}

void NoiseKernelStage::onProcess()
{
    bool regen = *regenerate;

    if (*dimensions != glm::ivec3(m_kernel.extent()))
    {
        resizeKernel();
        regen = true;
    }

    if (regen)
    {
        regenerateKernel();

        m_texture->image3D(0, gl::GL_RGB32F, *dimensions, 0, gl::GL_RGB, gl::GL_FLOAT, m_kernel.data());
        m_kernelData = {m_kernel.begin(), m_kernel.end()};
    }

    kernel.setValue(&m_kernelData);
    texture.setValue(m_texture.get());
}

void NoiseKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel3(*dimensions);
}

void NoiseKernelStage::regenerateKernel()
{
    glkernel::noise::uniform(m_kernel, 0.0f, 1.0f);
}


} // namespace gloperate_glkernel
