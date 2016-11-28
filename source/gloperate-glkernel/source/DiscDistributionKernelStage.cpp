#include <gloperate-glkernel/DiscDistributionKernelStage.h>

#include <algorithm>

#include <glm/vec2.hpp>

#include <glkernel/sample.h>
#include <glkernel/scale.h>
#include <glkernel/sort.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>
#include <globjects/Buffer.h>


namespace gloperate_glkernel
{


DiscDistributionKernelStage::DiscDistributionKernelStage(gloperate::Environment * environment)
: Stage(environment, "Disc Distribution Kernel")
, kernelSize("kernelSize", this, 1)
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
{
}


void DiscDistributionKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel2(static_cast<std::uint16_t>(*kernelSize));
}


void DiscDistributionKernelStage::regenerateKernel()
{
    glkernel::sample::poisson_square(m_kernel);

    // push the corners of the square in to form a disc
    std::transform(m_kernel.begin(), m_kernel.end(), m_kernel.begin(), [](glm::vec2 v) -> glm::vec2
    {
        glm::vec2 maxVec = v / std::max(v.x, v.y);
        float maxLen = glm::length(maxVec);
        return v / maxLen; 
    });

    glkernel::sort::distance(m_kernel, {0.0f, 0.0f});
}


void DiscDistributionKernelStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_texture = new globjects::Texture(gl::GL_TEXTURE_1D);
}


void DiscDistributionKernelStage::onProcess(gloperate::AbstractGLContext * context)
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

        m_texture->image1D(0, gl::GL_RG32F, *kernelSize, 0, gl::GL_RG, gl::GL_FLOAT, m_kernel.data());

        kernel.setValue(m_kernel);
        texture.setValue(m_texture);
    }
}


}
