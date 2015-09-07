
#include "CubeScape.h"

#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/resources/RawFile.h>

#include <gloperate/base/RenderTargetType.h>

#include <gloperate/painter/Camera.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>


using namespace gl;
using namespace glm;
using namespace globjects;


CubeScape::CubeScape(gloperate::ResourceManager & resourceManager, const std::string & relDataPath)
: Painter("CubeScape", resourceManager, relDataPath)
, m_animation{true}
, m_numCubes{25}
, a_vertex{-1}
, u_transform{-1}
, u_time{-1}
, u_numcubes{-1}
{
    // Setup painter
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability());
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
    m_projectionCapability = addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability));
    m_typedRenderTargetCapability = addCapability(new gloperate::TypedRenderTargetCapability());
    m_cameraCapability = addCapability(new gloperate::CameraCapability(glm::vec3(0.0, 2.0, 1.5), glm::vec3(0.0, 0.0, 0.5), glm::vec3(0.0, 1.0, 0.0)));
    m_timeCapability = addCapability(new gloperate::VirtualTimeCapability());
    
    m_timeCapability->setLoopDuration(20.0f * pi<float>());

    m_targetFramebufferCapability->changed.connect([this](){ this->onTargetFramebufferChanged();});

    // Register properties
    addProperty<bool>("Animation", this, &CubeScape::animation, &CubeScape::setAnimation);

    auto * propNumCubes = addProperty<int>("NumCubes", this, &CubeScape::numberOfCubes, &CubeScape::setNumberOfCubes);
    propNumCubes->setOption("minimum", 0);

    // Register scripting functions
    addFunction("randomize", this, &CubeScape::randomize);
}

CubeScape::~CubeScape()
{
}

void CubeScape::setupProjection()
{
    m_projectionCapability->setZNear(0.3f);
    m_projectionCapability->setZFar(15.f);
    m_projectionCapability->setFovy(radians(50.f));
}

void CubeScape::onInitialize()
{
    // create program

    globjects::init();
    onTargetFramebufferChanged();

#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    m_program = new globjects::Program;
    m_program->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER,   m_relDataPath + "data/cubescape/cubescape.vert"),
        globjects::Shader::fromFile(GL_GEOMETRY_SHADER, m_relDataPath + "data/cubescape/cubescape.geom"),
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, m_relDataPath + "data/cubescape/cubescape.frag")
    );

    // create textures

    m_textures[0] = new globjects::Texture;
    m_textures[1] = new globjects::Texture;

    for (int i=0; i < 2; ++i)
    {
        m_textures[i]->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_textures[i]->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

        m_textures[i]->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_textures[i]->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    {
        gloperate::RawFile terrain(m_relDataPath + "data/cubescape/terrain.512.512.r.ub.raw");
        if (!terrain.isValid())
            std::cout << "warning: loading texture from " << terrain.filePath() << " failed.";

        m_textures[0]->image2D(0, GL_RED_NV, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, terrain.data());
    }

    {
        gloperate::RawFile patches(m_relDataPath + "data/cubescape/patches.64.16.rgb.ub.raw");
        if (!patches.isValid())
            std::cout << "warning: loading texture from " << patches.filePath() << " failed.";

        m_textures[1]->image2D(0, GL_RGB8, 64, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, patches.data());
    }

    m_vao = new globjects::VertexArray;
    m_vao->bind();

    m_vertices = new globjects::Buffer;
    m_indices = new globjects::Buffer;

    const std::array<vec3, 8> vertices = 
    {
        vec3(-1.f, -1.f, -1.f ), // 0
        vec3(-1.f, -1.f,  1.f ), // 1
        vec3(-1.f,  1.f, -1.f ), // 2
        vec3(-1.f,  1.f,  1.f ), // 3
        vec3( 1.f, -1.f, -1.f ), // 4
        vec3( 1.f, -1.f,  1.f ), // 5
        vec3( 1.f,  1.f, -1.f ), // 6
        vec3( 1.f,  1.f,  1.f )  // 7
    };

    m_vertices->setData(vertices, GL_STATIC_DRAW);

    const std::array<unsigned char, 18> indices =
        { 2, 3, 6, 0, 1, 2, 1, 5, 3, 5, 4, 7, 4, 0, 6, 5, 1, 4 };

    m_indices->setData(indices, GL_STATIC_DRAW);
    m_indices->bind(GL_ELEMENT_ARRAY_BUFFER);

    // setup uniforms

    a_vertex = m_program->getAttributeLocation("a_vertex");

    m_vao->binding(0)->setAttribute(a_vertex);
    m_vao->binding(0)->setBuffer(m_vertices, 0, sizeof(vec3));
    m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);

    m_vao->enable(a_vertex);

    u_transform = m_program->getUniformLocation("modelViewProjection");
    u_time = m_program->getUniformLocation("time");
    u_numcubes = m_program->getUniformLocation("numcubes");

    GLint terrain = m_program->getUniformLocation("terrain");
    GLint patches = m_program->getUniformLocation("patches");

    // since only single program and single data is used, bind only once 

    glClearColor(0.f, 0.f, 0.f, 1.0f);

    m_program->setUniform(terrain, 0);
    m_program->setUniform(patches, 1);

    setupProjection();
}

void CubeScape::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        glViewport(m_viewportCapability->x(), m_viewportCapability->y(), m_viewportCapability->width(), m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(GL_FRAMEBUFFER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    mat4 transform = m_projectionCapability->projection() * m_cameraCapability->view();

    m_vao->bind();

    m_indices->bind(GL_ELEMENT_ARRAY_BUFFER);

    m_program->use();

    m_program->setUniform(u_transform, transform);
    m_program->setUniform(u_time, m_timeCapability->time());
    m_program->setUniform(u_numcubes, m_numCubes);

    m_textures[0]->bindActive(GL_TEXTURE0);
    m_textures[1]->bindActive(GL_TEXTURE1);

    m_vao->drawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, nullptr, m_numCubes * m_numCubes);

    m_program->release();
    m_vao->unbind();

    globjects::Framebuffer::unbind(GL_FRAMEBUFFER);
}

bool CubeScape::animation() const
{
    return m_animation;
}

void CubeScape::setAnimation(const bool & enabled)
{
    m_animation = enabled;

    m_timeCapability->setEnabled(m_animation);
}

int CubeScape::numberOfCubes() const
{
    return m_numCubes;
}

void CubeScape::setNumberOfCubes(const int & number)
{
    m_numCubes = number;
}

void CubeScape::onTargetFramebufferChanged()
{
    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();
    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }
    m_typedRenderTargetCapability->setRenderTarget(gloperate::RenderTargetType::Depth, fbo, gl::GLenum::GL_DEPTH_ATTACHMENT, gl::GLenum::GL_DEPTH_COMPONENT);
}

void CubeScape::randomize()
{
    setNumberOfCubes(rand() % 40 + 1);
}
