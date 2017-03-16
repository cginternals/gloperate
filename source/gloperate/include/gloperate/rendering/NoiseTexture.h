#pragma once

#include <vector>
#include <memory>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class GLOPERATE_API NoiseTexture
{
public:
    NoiseTexture(
        unsigned int inputDimensions,
        unsigned int outputDimensions,
        gl::GLsizei textureSize = 64);

    globjects::Texture *texture();

    void bindActive(unsigned int index) const;
    void unbindActive(unsigned int index) const;


protected:
    static gl::GLenum targetForDimensions(unsigned int dimensions);
    static gl::GLenum internalFormatForDimensions(unsigned int dimensions);
    static gl::GLenum formatForDimensions(unsigned int dimensions);

    static std::unique_ptr<globjects::Texture> createTexture(
        unsigned int inputDimensions,
        unsigned int outputDimensions,
        gl::GLsizei textureSize);

    static std::vector<float> generateNoise(
        unsigned int inputDimensions,
        unsigned int outputDimensions,
        gl::GLsizei textureSize);


protected:
    std::unique_ptr<globjects::Texture> m_texture;
};


} // namespace gloperate
