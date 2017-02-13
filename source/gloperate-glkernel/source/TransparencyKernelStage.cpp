
#include <gloperate-glkernel/TransparencyKernelStage.h>

#include <algorithm>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(TransparencyKernelStage, gloperate::Stage)


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
, m_texture(nullptr)
{
}


void TransparencyKernelStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_texture = new globjects::Texture(gl::GL_TEXTURE_2D);
}


void TransparencyKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    if (!m_texture)
    {
        onContextInit(context);
    }

    if (*regenerate)
    {
        const size_t alphaValues = 256;
        const size_t maskSize = 1024;
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

        m_texture->image2D(1, gl::GL_R8, maskSize, alphaValues, 0, gl::GL_R, gl::GL_BYTE, m_kernelData.data());
    }

    kernel.setValue(&m_kernelData);
    texture.setValue(m_texture);
}


} // namespace gloperate_glkernel
