
#include <gloperate-glkernel/TransparencyKernelStage.h>

#include <algorithm>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(TransparencyKernelStage, gloperate::Stage)


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
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

    const size_t alphaValues = 256;
    const size_t maskSize = 1024;
    std::vector<unsigned char> newKernel(maskSize * alphaValues);

    for (auto alphaIndex = 0; alphaIndex < alphaValues; alphaIndex++)
    {
        auto alphaVal = float(alphaIndex) / alphaValues;
        auto numHits = int(std::floor(alphaVal * maskSize));

        auto lineBegin   = newKernel.begin() +  alphaIndex      * maskSize;
        auto lineEnd     = newKernel.begin() + (alphaIndex + 1) * maskSize;
        auto changePoint = lineBegin + numHits;

        std::fill(lineBegin  , changePoint, 255);
        std::fill(changePoint, lineEnd    , 0);
        std::random_shuffle(lineBegin, lineEnd);
    }

    m_texture->image2D(1, gl::GL_R8, maskSize, alphaValues, 0, gl::GL_R, gl::GL_BYTE, newKernel.data());

    kernel.setValue(newKernel);
    texture.setValue(m_texture);
}


} // namespace gloperate_glkernel
