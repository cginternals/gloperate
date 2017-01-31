#include <gloperate/rendering/NoiseTexture.h>

#include <cassert>
#include <cmath>
#include <algorithm>

#include <glm/gtc/random.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate
{

NoiseTexture::NoiseTexture(
    unsigned int inputDimensions,
    unsigned int outputDimensions,
    gl::GLsizei textureSize)
{
    m_texture = createTexture(inputDimensions, outputDimensions, textureSize);
}

void NoiseTexture::bindActive(unsigned int index) const
{
    m_texture->bindActive(index);
}

void NoiseTexture::unbindActive(unsigned int index) const
{
    m_texture->unbindActive(index);
}

globjects::Texture * NoiseTexture::texture()
{
    return m_texture;
}

gl::GLenum NoiseTexture::targetForDimensions(unsigned int dimensions)
{
    assert(dimensions >= 1 && dimensions <= 3);

    switch (dimensions)
    {
    case 1u:
        return gl::GL_TEXTURE_1D;
    case 2u:
        return gl::GL_TEXTURE_2D;
    case 3u:
        return gl::GL_TEXTURE_3D;
    default:
        return gl::GL_NONE;
    }
}

gl::GLenum NoiseTexture::internalFormatForDimensions(unsigned int dimensions)
{
    assert(dimensions >= 1 && dimensions <= 4);

    switch (dimensions)
    {
    case 1u:
        return gl::GL_R32F;
    case 2u:
        return gl::GL_RG32F;
    case 3u:
        return gl::GL_RGB32F;
    case 4u:
        return gl::GL_RGBA32F;
    default:
        return gl::GL_NONE;
    }
}

gl::GLenum NoiseTexture::formatForDimensions(unsigned int dimensions)
{
    assert(dimensions >= 1 && dimensions <= 4);

    switch (dimensions)
    {
    case 1u:
        return gl::GL_RED;
    case 2u:
        return gl::GL_RG;
    case 3u:
        return gl::GL_RGB;
    case 4u:
        return gl::GL_RGBA;
    default:
        return gl::GL_NONE;
    }
}

globjects::Texture * NoiseTexture::createTexture(
    unsigned int inputDimensions,
    unsigned int outputDimensions,
    gl::GLsizei textureSize)
{
    const auto target = targetForDimensions(inputDimensions);
    const auto internalFormat = internalFormatForDimensions(outputDimensions);
    const auto format = formatForDimensions(outputDimensions);

    auto noise = generateNoise(inputDimensions, outputDimensions, textureSize);

    auto texture = globjects::Texture::createDefault(target);

    switch (target)
    {
    case gl::GL_TEXTURE_1D:
        texture->image1D(0, internalFormat, textureSize,
            0, format, gl::GL_FLOAT, noise.data());
        break;
    case gl::GL_TEXTURE_2D:
        texture->image2D(0, internalFormat, textureSize, textureSize,
            0, format, gl::GL_FLOAT, noise.data());
        break;
    case gl::GL_TEXTURE_3D:
        texture->image3D(0, internalFormat, textureSize, textureSize, textureSize,
            0, format, gl::GL_FLOAT, noise.data());
        break;
    default:
        break;
    }

    texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
    texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);
    texture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_REPEAT);

    return texture;
}

std::vector<float> NoiseTexture::generateNoise(
    unsigned int inputDimensions,
    unsigned int outputDimensions,
    gl::GLsizei textureSize)
{
    const auto size = std::pow(textureSize, inputDimensions) * outputDimensions;

    auto noiseArray = std::vector<float>(size);
    std::generate(noiseArray.begin(), noiseArray.end(), []() { return glm::linearRand(0.0f, 1.0f); });

    return noiseArray;
}


} // namespace gloperate
