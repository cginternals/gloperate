#include <basic-examples/CubeScape/CubeScape.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/DebugMessage.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/Viewport.h>

using namespace gl;

// -- rawfile.cpp

#include <fstream>
#include <iostream>

namespace {
    static const char * vertSource = R"(
#version 150 core

in vec3 a_vertex;
out float v_h;

uniform sampler2D terrain;
uniform float time;

uniform int numcubes;

void main()
{
    float oneovernumcubes = 1.f / float(numcubes);
    vec2 uv = vec2(mod(gl_InstanceID, numcubes), int(gl_InstanceID * oneovernumcubes)) * 2.0 * oneovernumcubes;

    vec3 v = 2.0 * a_vertex * oneovernumcubes;
    v.xz += uv * 2.0 - vec2(2.0);

    v_h = texture2D(terrain, uv * 0.6 + vec2(sin(time * 0.04), time * 0.02)).r;
    v.y += v_h;

    gl_Position = vec4(v, 1.0);
})";

    static const char * geomSource = R"(
#version 150 core

uniform mat4 modelViewProjection;

in  float v_h[3];
out float g_h;

out vec2 g_uv;
out vec3 g_normal;

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

void main()
{
    vec4 u = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 v = gl_in[2].gl_Position - gl_in[0].gl_Position;

    vec3 n = cross(normalize((modelViewProjection * u).xyz), normalize((modelViewProjection * v).xyz));

    gl_Position = modelViewProjection * gl_in[0].gl_Position;
    g_uv = vec2(0.0, 0.0);
    g_normal = n;
    g_h = v_h[0];
    EmitVertex();

    gl_Position = modelViewProjection * gl_in[1].gl_Position;
    g_uv = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = modelViewProjection * gl_in[2].gl_Position;
    g_uv = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = modelViewProjection * vec4((gl_in[0].gl_Position + u + v).xyz, 1.0);
    g_uv = vec2(1.0, 1.0);
    EmitVertex();
})";

    static const char * fragSource = R"(
#version 150 core

in float g_h;

in vec2 g_uv;
in vec3 g_normal;

out vec4 fragColor;

uniform sampler2D patches;

void main()
{
    vec3 n = normalize(g_normal);
    vec3 l = normalize(vec3(0.0, -0.5, 1.0));

    float lambert = dot(n, l);

    float t = (1.0 - g_h) * 4.0 - 1.0;
    vec2 uv = g_uv * vec2(0.25, 1.0);

    vec4 c0 = texture2D(patches, uv + max(floor(t), 0.0) * vec2(0.25, 0.0));
    vec4 c1 = texture2D(patches, uv + min(floor(t) + 1.0, 3.0) * vec2(0.25, 0.0));

    fragColor = mix(c0, c1, smoothstep(0.25, 0.75, fract(t))) * lambert;
})";

}

RawFile::RawFile(const std::string & _filePath)
    : m_filePath(_filePath)
    , m_valid(false)
{
    m_valid = readFile();
}

RawFile::~RawFile()
{
}

bool RawFile::isValid() const
{
    return m_valid;
}

const char * RawFile::data() const
{
    return m_data.data();
}

size_t RawFile::size() const
{
    return m_data.size();
}

bool RawFile::readFile()
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
    {
        std::cerr << "Reading from file \"" << m_filePath << "\" failed." << std::endl;
        return false;
    }

    readRawData(ifs);

    ifs.close();

    return true;
}

void RawFile::readRawData(std::ifstream & ifs)
{
    ifs.seekg(0, std::ios::end);

    const size_t _size = static_cast<size_t>(ifs.tellg());
    m_data.resize(_size);

    ifs.seekg(0, std::ios::beg);
    ifs.read(m_data.data(), static_cast<std::streamsize>(_size));
}


// -- cubescape.cpp

CubeScape::CubeScape(gloperate::ResourceManager * /*resourceManager*/)
: a_vertex(-1)
, u_transform(-1)
, u_time(-1)
, u_numcubes(-1)
, m_a(0.f)
{
}

CubeScape::~CubeScape()
{
}

void CubeScape::onInitialize()
{
    // create program

    m_program = new globjects::Program;
    m_program->attach(
        globjects::Shader::fromString(GL_VERTEX_SHADER, vertSource),
        globjects::Shader::fromString(GL_GEOMETRY_SHADER, geomSource),
        globjects::Shader::fromString(GL_FRAGMENT_SHADER, fragSource)
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
        RawFile terrain("data/cubescape/terrain.512.512.r.ub.raw");
        if (!terrain.isValid())
            std::cout << "warning: loading texture from " << terrain.filePath() << " failed.";

        m_textures[0]->image2D(0, GL_LUMINANCE8, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, terrain.data());
    }

    {
        RawFile patches("data/cubescape/patches.64.16.rgb.ub.raw");
        if (!patches.isValid())
            std::cout << "warning: loading texture from " << patches.filePath() << " failed.";

        m_textures[1]->image2D(0, GL_RGB8, 64, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, patches.data());
    }

    m_vao = new globjects::VertexArray;
    m_vao->bind();

    m_vertices = new globjects::Buffer;
    m_indices = new globjects::Buffer;

    m_vertices->setData(std::array<glm::vec3, 8>{
        glm::vec3(-1.f, -1.f, -1.f), // 0
        glm::vec3(-1.f, -1.f,  1.f), // 1
        glm::vec3(-1.f,  1.f, -1.f), // 2
        glm::vec3(-1.f,  1.f,  1.f), // 3
        glm::vec3( 1.f, -1.f, -1.f), // 4
        glm::vec3( 1.f, -1.f,  1.f), // 5
        glm::vec3( 1.f,  1.f, -1.f), // 6
        glm::vec3( 1.f,  1.f,  1.f)  // 7
    }, GL_STATIC_DRAW);

    m_indices->setData(std::array<unsigned char, 18>{ 2, 3, 6, 0, 1, 2, 1, 5, 3, 5, 4, 7, 4, 0, 6, 5, 1, 4 }, GL_STATIC_DRAW);
    m_indices->bind(GL_ELEMENT_ARRAY_BUFFER);

    // setup uniforms

    a_vertex = m_program->getAttributeLocation("a_vertex");

    m_vao->binding(0)->setAttribute(a_vertex);
    m_vao->binding(0)->setBuffer(m_vertices, 0, sizeof(glm::vec3));
    m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);

    m_vao->enable(a_vertex);

    u_transform = m_program->getUniformLocation("modelViewProjection");
    u_time = m_program->getUniformLocation("time");
    u_numcubes = m_program->getUniformLocation("numcubes");

    //GLint terrain = m_program->getUniformLocation("terrain");
    //GLint patches = m_program->getUniformLocation("patches");

    // since only single program and single data is used, bind only once 

    glClearColor(0.f, 0.f, 0.f, 1.0f);

    //m_program->setUniform(terrain, 0);
    m_program->setUniform("terrain", 0);
    //m_program->setUniform(patches, 1);
    m_program->setUniform("patches", 1);

    // create fbo for result

    /*m_tex = new globjects::Texture;
    m_tex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_tex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_tex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_tex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_tex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_z = new globjects::Renderbuffer;

    m_fbo = new globjects::Framebuffer;

    m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, m_tex);
    m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_z);*/

    // view

    m_view = glm::lookAt(glm::vec3(0.f, 4.f, -4.f), glm::vec3(0.f, -0.6f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

void CubeScape::onResize(const gloperate::Viewport & viewport)
{
    glViewport(viewport.x(), viewport.y(), viewport.width(), viewport.height());

    //m_z->storage(GL_DEPTH_COMPONENT16, viewport.width(), viewport.height());
    //m_tex->image2D(0, GL_RGBA, viewport.width(), viewport.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

    m_projection = glm::perspective(glm::radians(50.f), static_cast<GLfloat>(viewport.width()) / static_cast<GLfloat>(viewport.height()), 2.f, 8.f);
}

void CubeScape::onPaint()
{
    const int numcubes = 25;
    const float t = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 transform = m_projection * m_view;// * glm::rotate(glm::mat4(), t * 0.1f, glm::vec3(0.f, 1.f, 0.f));

    m_vao->bind();

    m_indices->bind(GL_ELEMENT_ARRAY_BUFFER);

    m_program->use();

    //m_program->setUniform(u_transform, transform);
    m_program->setUniform("modelViewProjection", transform);
    //m_program->setUniform(u_time, t);
    m_program->setUniform("time", t);
    //m_program->setUniform(u_numcubes, numcubes);
    m_program->setUniform("numcubes", numcubes);

    m_textures[0]->bindActive(GL_TEXTURE0);
    m_textures[1]->bindActive(GL_TEXTURE1);

    m_vao->drawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, nullptr, numcubes * numcubes);

    m_program->release();
    m_vao->unbind();
}
