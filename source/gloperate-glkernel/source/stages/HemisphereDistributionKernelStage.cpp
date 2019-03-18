
#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>

#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>
#include <globjects/Buffer.h>

#include <glkernel/sample.h>
#include <glkernel/scale.h>
#include <glkernel/sort.h>
#include <glkernel/shuffle.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(HemisphereDistributionKernelStage, gloperate::Stage)


HemisphereDistributionKernelStage::HemisphereDistributionKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, kernelSize("kernelSize", this, 1)
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
{
}

HemisphereDistributionKernelStage::~HemisphereDistributionKernelStage()
{
}

void HemisphereDistributionKernelStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_1D);

    texture.invalidate();
}

void HemisphereDistributionKernelStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_texture.reset();

    texture.setValue(nullptr);
}

void HemisphereDistributionKernelStage::onProcess()
{
    bool regenKernel = *regenerate;
    if (*kernelSize != m_kernel.extent().x)
    {
        resizeKernel();
        regenKernel = true;
    }

    if (regenKernel)
    {
        regenerateKernel();

        m_texture->image1D(0, gl::GL_RGB32F, *kernelSize, 0, gl::GL_RGB, gl::GL_FLOAT, m_kernel.data());

        m_kernelData = {m_kernel.begin(), m_kernel.end()};
        kernel.setValue(&m_kernelData);
        texture.setValue(m_texture.get());
    }
}

void HemisphereDistributionKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel3(static_cast<std::uint16_t>(*kernelSize));
}

void HemisphereDistributionKernelStage::regenerateKernel()
{
    glkernel::sample::hammersley_sphere(m_kernel);

    glkernel::shuffle::random(m_kernel);

    std::transform(m_kernel.begin(), m_kernel.end(), m_kernel.begin(), [](const glm::vec3 & v){
        return v * glm::linearRand(0.1f, 1.0f);
    });
}


} // namespace gloperate_glkernel
