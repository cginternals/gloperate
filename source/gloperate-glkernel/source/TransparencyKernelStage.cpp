
#include <gloperate-glkernel/TransparencyKernelStage.h>

#include <algorithm>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate_glkernel
{


TransparencyKernelStage::TransparencyKernelStage(gloperate::Environment * environment)
: Stage(environment, "Transparency Kernel")
, kernel("kernel", this)
, texture("texture", this)
{
}


void TransparencyKernelStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_texture = new globjects::Texture(gl::GL_TEXTURE_2D);
}


void TransparencyKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    const size_t alphaValues = 256;
    const size_t maskSize = 1024;
    std::array<unsigned char, maskSize * alphaValues> kernel;

    for (auto alphaIndex = 0; alphaIndex < alphaValues; alphaIndex++)
    {
        auto alphaVal = float(alphaIndex) / alphaValues;
        auto numHits = int(std::floor(alphaVal * maskSize));

        auto lineBegin   = kernel.begin() +  alphaIndex      * maskSize;
        auto lineEnd     = kernel.begin() + (alphaIndex + 1) * maskSize;
        auto changePoint = lineBegin + numHits;

        std::fill(lineBegin  , changePoint, 255);
        std::fill(changePoint, lineEnd    , 0);
        std::random_shuffle(lineBegin, lineEnd);
    }

    m_texture->image2D(1, gl::GL_R8, maskSize, alphaValues, 0, gl::GL_R, gl::GL_BYTE, kernel.data());
}


}
