
#pragma once


#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Shader.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Screen aligned quad rendering helper
*
*    This class can be used to render a screen aligned quad.
*    It is configurable with regard to the used texture,
*    shader programs, and uniform values.
*/
class GLOPERATE_API ScreenAlignedQuad : public globjects::Referenced
{
public:
    ScreenAlignedQuad(globjects::Shader * fragmentShader, globjects::Texture * texture);
    ScreenAlignedQuad(globjects::Texture * texture);
    ScreenAlignedQuad(globjects::Shader  * fragmentShader);
    ScreenAlignedQuad(globjects::Program * program);

    void draw();

    globjects::Program * program();

    globjects::Shader * vertexShader();
    globjects::Shader * fragmentShader();

    void setTexture(globjects::Texture * texture);

    void setSamplerUniform(int index);

    globjects::VertexArray * vao() const;


protected:
    void initialize();


protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;

    globjects::ref_ptr<globjects::Buffer> m_buffer;

    globjects::ref_ptr<globjects::Shader> m_vertexShader;
    globjects::ref_ptr<globjects::Shader> m_fragmentShader;

    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::Texture> m_texture;

    int m_samplerIndex;


protected:
    static const char * s_defaultVertexShaderSource;
    static const char * s_defaultFagmentShaderSource;
};


} // namespace gloperate
