
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>

#include <algorithm>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(TransparencyKernelStage, gloperate::Stage)


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, kernelSize("kernelSize", this, glm::ivec2(1))
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
{
}

TransparencyKernelStage::~TransparencyKernelStage()
{
}

void TransparencyKernelStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_texture = globjects::Texture::create(gl::GL_TEXTURE_2D);

    m_texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
    m_texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);
    m_texture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_REPEAT);

    texture.invalidate();
}

void TransparencyKernelStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_texture.reset();

    texture.setValue(nullptr);
}

void TransparencyKernelStage::onProcess()
{
    if (*regenerate)
    {
        regenerateKernel();
        m_texture->image2D(0, gl::GL_R8, *kernelSize, 0, gl::GL_RED, gl::GL_UNSIGNED_BYTE, m_kernelData.data());
    }

    kernel.setValue(&m_kernelData);
    texture.setValue(m_texture.get());
}

void TransparencyKernelStage::regenerateKernel()
{
    const auto maskSize = (*kernelSize).x;
    const auto alphaValues = (*kernelSize).y;

    m_kernelData = std::vector<unsigned char>(maskSize * alphaValues);

    for (auto alphaIndex = 0; alphaIndex < alphaValues; alphaIndex++)
    {
        auto alphaVal = float(alphaIndex) / alphaValues;
        auto numHits = int(std::floor(alphaVal * maskSize));

        auto lineBegin   = m_kernelData.begin() +  alphaIndex      * maskSize;
        auto lineEnd     = m_kernelData.begin() + (alphaIndex + 1) * maskSize;
        auto changePoint = lineBegin + numHits;

        std::fill(lineBegin  , changePoint, 255);
        std::fill(changePoint, lineEnd    , 0);

        std::random_shuffle(lineBegin, lineEnd);
    }
}


} // namespace gloperate_glkernel
