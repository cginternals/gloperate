
#pragma once


#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <glbinding/gl/types.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>

namespace globjects
{
	class Framebuffer;
	class Program;
	class Texture;
	class Shader;
	class VertexArray;
}

namespace gloperate
{

class PolygonalDrawable;
class AbstractTargetFramebufferCapability;
class AbstractViewportCapability;


/**
*  @brief
*    A rendering helper for textured objects.
*
*    On construction of TexturedObject, an object has to be provided as either PolygonalDrawable or VertexArray.
*    On each call to the draw() method, that object is rendered using either the default program provided by the 
*    TexturedObject class or a user defined program. 
*    The user may optionally specify a color texture, a normal texture and/or a height texture to be used for the
*    rendering. If provided, the normal texture is used for diffuse lighting and the height texture is used for
*    per pixel displacement mapping 
*    (see "GPU Gems 2", chapter 8; http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter08.html). 
*    The user can turn the usage of each of the textures on or off individually at any time. The per pixel 
*    displacement mapping algorithm in use has some parameters which can be set by the user at any time. In addition,
*    the TexturedObject class provides reasonable default values.
*
*    Transparency in the provided color texture is supported.
*
*    Note: Whenever a new height texture is set or when the value of the depth layers parameter is changed, 
*    a 3D distance texture is computed from the set height texture for the displacement mapping.
*    Depending on the size of the height texture and the displacement mapping parameters this may take a few seconds.
*/
class GLOPERATE_API TexturedObject : public globjects::Referenced
{
public:
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Shader * vertexShader, globjects::Shader * fragmentShader);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Program * program);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Shader * fragmentShader);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Shader * vertexShader, globjects::Shader * fragmentShader);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Program * program);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Shader * fragmentShader);
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount);

    void draw();

	/* getters and setters */
	void setTransform(glm::mat4 transform);
	void setEye(glm::vec3 eye);

	globjects::Program * program() const;
	globjects::Shader * vertexShader() const;
	globjects::Shader * fragmentShader() const;

	void setColorTexture(globjects::Texture * texture);
	const globjects::Texture * colorTexture() const;
	globjects::Texture * colorTexture();

	void setNormalTexture(globjects::Texture * texture);
	const globjects::Texture * normalTexture() const;
	globjects::Texture * normalTexture();

	void setHeightTexture(globjects::Texture * texture);
	const globjects::Texture * heightTexture() const;
	globjects::Texture * heightTexture();

	void setColorSamplerUniform(int index);
	void setNormalSamplerUniform(int index);
	void setDistanceSamplerUniform(int index);
	void setMaskSamplerUniform(int index);

	PolygonalDrawable * polygonalDrawable() const;
	globjects::VertexArray * vertexArray() const;

	void setIterations(unsigned short iterations);
	unsigned short iterations() const;
	void setLightPosition(glm::vec3 position);
	glm::vec3 lightPosition() const;
	void setUseColors(bool useColors);
	bool useColors() const;
	void setUseNormals(bool useNormals);
	bool useNormals() const;
	void setUseHeights(bool useHeights);
	bool useHeights() const;
	void setInverseBumpDepth(float invBumpDepth);
	float inverseBumpDepth() const;
	void setDepthLayers(unsigned short depthLayers);
	unsigned short depthLayers() const;
	/* end of getters and setters */

protected:
	TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, PolygonalDrawable * drawable, globjects::Shader * vertexShader, globjects::Shader * fragmentShader, globjects::Program * program);
	void initialize();
	void updateDistanceTexture();

	void setupFramebuffer();
	void setupMaskTexture();
	void updateFramebuffer();
	void clearBuffers();
	void renderOpaqueGeometry();
	void renderAlphaToCoverage(gl::GLenum colorAttachment);
	void blit();

protected:
	PolygonalDrawable * m_polygonalDrawable;
	globjects::ref_ptr<globjects::VertexArray> m_vao;
	int m_vaoElementCount;
	
	globjects::ref_ptr<globjects::Program> m_program;
	globjects::ref_ptr<globjects::Shader> m_vertexShader;
	globjects::ref_ptr<globjects::Shader> m_fragmentShader;
	
	glm::mat4 m_transform;
	glm::vec3 m_eye;

	globjects::ref_ptr<globjects::Texture> m_colorTexture;
	globjects::ref_ptr<globjects::Texture> m_normalTexture;
	globjects::ref_ptr<globjects::Texture> m_heightTexture;
	globjects::ref_ptr<globjects::Texture> m_distanceTexture;
	globjects::ref_ptr<globjects::Texture> m_maskTexture;

	int m_colorSamplerIndex;
	int m_normalSamplerIndex;
	int m_distanceSamplerIndex;
	int m_maskSamplerIndex;

	gl::GLint m_textureWidth;
	gl::GLint m_textureHeight;

	unsigned short m_iterations;
	glm::vec3 m_lightPosition;
	bool m_useColors;
	bool m_useNormals;
	bool m_useHeights;
	float m_inverseBumpDepth;
	unsigned short m_depthLayers;

	globjects::ref_ptr<globjects::Framebuffer> m_fbo;
	globjects::ref_ptr<globjects::Texture> m_opaqueColorAttachment;
	globjects::ref_ptr<globjects::Texture> m_depthAttachment;

	gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
	gloperate::AbstractViewportCapability * m_viewportCapability;

protected:
    static const char * s_defaultVertexShaderSource;
    static const char * s_defaultFagmentShaderSource;
	static const int s_numSamples = 8; /* number of samples for the transparency multisampling */
};

} // namespace gloperate
