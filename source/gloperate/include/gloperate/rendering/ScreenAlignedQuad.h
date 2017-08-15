
#pragma once


#include <gloperate/rendering/Drawable.h>


namespace globjects
{
    class Buffer;
    class Texture;
    class Program;
    class Shader;
    class AbstractStringSource;
}


namespace gloperate
{


/**
*  @brief
*    Screen aligned quad rendering helper
*
*    This class can be used to render a screen aligned quad.
*/
class GLOPERATE_API ScreenAlignedQuad : public AbstractDrawable
{
public:
    /**
    *  @brief
    *    Get vertex shader for screen aligned quad
    *
    *  @return
    *    String source for shader
    */
    static std::unique_ptr<globjects::AbstractStringSource> vertexShaderSource();

    /**
    *  @brief
    *    Get fragment shader for screen aligned quad
    *
    *  @return
    *    String source for shader
    */
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSource();

    /**
    *  @brief
    *    Get fragment shader for screen aligned quad
    *
    *  @return
    *    String source for shader
    */
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSourceInverted();


public:
    /**
    *  @brief
    *    Constructor
    */
    ScreenAlignedQuad();

    /**
    *  @brief
    *    Destructor
    *
    *  @remarks
    *    Destroys OpenGL objects, thus, a current context is required.
    */
    virtual ~ScreenAlignedQuad();

    /**
    *  @brief
    *    Get texture
    *
    *  @return
    *    Texture that is displayed (can be null)
    */
    globjects::Texture * texture() const;

    /**
    *  @brief
    *    Set texture
    *
    *  @param[in] texture
    *    Texture that is displayed (can be null)
    */
    void setTexture(globjects::Texture * texture);

    /**
    *  @brief
    *    Check if texture is mirrored vertically
    *
    *  @return
    *    'true' if texture is mirrored vertically, else 'false'
    */
    bool inverted() const;

    /**
    *  @brief
    *    Set if texture is mirrored vertically
    *
    *  @param[in] inverted
    *    'true' if texture is mirrored vertically, else 'false'
    */
    void setInverted(bool inverted);

    /**
    *  @brief
    *    Draw geometry
    */
    virtual void draw() const override;


protected:
    /**
    *  @brief
    *    Initialize OpenGL objects
    */
    void initialize();


protected:
    std::unique_ptr<Drawable>                          m_drawable;             ///< Underlying drawable object
    std::unique_ptr<globjects::Buffer>                 m_vertices;             ///< Vertex buffer
    std::unique_ptr<globjects::AbstractStringSource>   m_vertexShaderSource;   ///< Vertex shader source
    std::unique_ptr<globjects::AbstractStringSource>   m_fragmentShaderSource; ///< Fragment shader source
    std::unique_ptr<globjects::Shader>                 m_vertexShader;         ///< Vertex shader
    std::unique_ptr<globjects::Shader>                 m_fragmentShader;       ///< Fragment shader
    std::unique_ptr<globjects::Program>                m_program;              ///< Shader program
    globjects::Texture                               * m_texture;              ///< Texture that is displayed (can be null)
    bool                                               m_inverted;             ///< If 'true', the texture will be mirrored vertically, else 'false'
    bool                                               m_initialized;          ///< 'true' if initialized, else 'false'
};


} // namespace gloperate
