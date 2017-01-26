
#include <gloperate/stages/multiframe/SSAOKernelStage.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace
{

std::vector<glm::vec3> ssaoKernel(unsigned int size)
{
    static const auto minDistance = 0.1f;
    const auto inverseSize = 1.f / static_cast<float>(size);

    auto kernel = std::vector<glm::vec3>();

    kernel.reserve(size);

    if (size == 1)
    {
        kernel.push_back(glm::vec3(0.f));

        return kernel;
    }

    while (kernel.size() < size)
    {
        auto v = glm::sphericalRand(1.f);
        v.z = glm::abs(v.z);
        if (v.z < 0.1)
            continue;

        auto scale = static_cast<float>(kernel.size()) * inverseSize;
        scale = scale * scale * (1.f - minDistance) + minDistance;

        v *= scale;

        kernel.push_back(v);
    }

    return kernel;
}

std::vector<glm::vec2> ssaoNoise(const unsigned int size)
{
    auto kernel = std::vector<glm::vec2>();

    for(auto y = 0u; y < size; ++y)
    {
        for(auto x = 0u; x < size; ++x)
        {
            kernel.push_back(glm::circularRand(1.f));
        }
    }

    return kernel;
}

} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(SSAOKernelStage, gloperate::Stage)


SSAOKernelStage::SSAOKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, enable("enable", this, true)
, kernelSize("kernelSize", this)
, noiseSize("noiseSize", this)
, currentFrame("currentFrame", this)
, kernelTexture("ssaoTexture", this)
, noiseTexture("noiseTexture", this)
{
}

void SSAOKernelStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_kernelTexture = new globjects::Texture(gl::GL_TEXTURE_1D);
    m_kernelTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
    m_kernelTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
    m_kernelTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_MIRRORED_REPEAT);

    m_noiseTexture = new globjects::Texture(gl::GL_TEXTURE_2D);
    m_noiseTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
    m_noiseTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
    m_noiseTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
    m_noiseTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);

    noiseTexture.setValue(m_noiseTexture);
    kernelTexture.setValue(m_kernelTexture);
}

void SSAOKernelStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    if (*enable)
    {
        recreateKernels();
    }

    kernelTexture.setValid(true);
    noiseTexture.setValid(true);
}

void SSAOKernelStage::recreateKernels()
{
    m_kernelTexture->image1D(0, gl::GL_RGB16F, *kernelSize, 0, gl::GL_RGB, gl::GL_FLOAT, ssaoKernel(*kernelSize).data());
    m_noiseTexture->image2D(0, gl::GL_RG16F, *noiseSize, *noiseSize, 0, gl::GL_RG, gl::GL_FLOAT, ssaoNoise(*noiseSize).data());
}


} // namespace gloperate
