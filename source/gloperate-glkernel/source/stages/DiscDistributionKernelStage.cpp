
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include <algorithm>

#include <glm/vec2.hpp>

#include <glkernel/sample.h>
#include <glkernel/scale.h>
#include <glkernel/sort.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>
#include <globjects/Buffer.h>


namespace
{


using namespace glm;


// push the corners of the square in to form a disc
vec2 pushCorners(const vec2 & v)
{
    if (std::max(std::abs(v.x), std::abs(v.y)) < 1e-10f)
        return v;

    vec2 maxVec = v / std::max(std::abs(v.x), std::abs(v.y));
    float maxLen = length(maxVec);
    return v / maxLen;
}


}


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(DiscDistributionKernelStage, gloperate::Stage)


DiscDistributionKernelStage::DiscDistributionKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, kernelSize("kernelSize", this, 1)
, radius("radius", this, 1.0f)
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
{
}

DiscDistributionKernelStage::~DiscDistributionKernelStage()
{
}

void DiscDistributionKernelStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_1D);
}


void DiscDistributionKernelStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_texture.reset();
}


void DiscDistributionKernelStage::onProcess()
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

        m_kernelData = {m_kernel.begin(), m_kernel.end()};
        kernel.setValue(&m_kernelData);
        texture.setValue(m_texture.get());
    }
}

void DiscDistributionKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel2(static_cast<std::uint16_t>(*kernelSize));
}

void DiscDistributionKernelStage::regenerateKernel()
{
    glkernel::sample::poisson_square(m_kernel);

    const float r = *radius;
    glkernel::scale::range(m_kernel, -r, r);

    std::transform(m_kernel.begin(), m_kernel.end(), m_kernel.begin(), pushCorners);

    glkernel::sort::distance(m_kernel, glm::vec2(0.0f, 0.0f));
}


} // namespace gloperate_glkernel
