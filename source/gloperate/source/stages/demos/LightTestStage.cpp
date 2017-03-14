
#include <gloperate/stages/demos/LightTestStage.h>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/base/File.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/NamedString.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{


static const std::array<std::array<glm::vec3, 2>, 14> s_cube { {
    {{ glm::vec3(-1.f, -1.f, -1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(-1.f, -1.f, +1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, -1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, +1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, +1.f, +1.f), glm::vec3( 1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, -1.f, +1.f), glm::vec3( 0.0f,  0.0f,  1.0) }},
    {{ glm::vec3(-1.f, +1.f, +1.f), glm::vec3( 0.0f,  0.0f,  1.0) }},
    {{ glm::vec3(-1.f, -1.f, -1.f), glm::vec3(-1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, -1.f), glm::vec3(-1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, -1.f), glm::vec3( 0.0f,  0.0f, -1.0) }},
    {{ glm::vec3(+1.f, +1.f, -1.f), glm::vec3( 0.0f,  0.0f, -1.0) }},
    {{ glm::vec3(+1.f, +1.f, +1.f), glm::vec3( 1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, -1.f), glm::vec3( 0.0f,  1.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, +1.f), glm::vec3( 0.0f,  1.0f,  0.0) }}
} };

static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform mat4 modelMatrix;
    uniform mat4 modelViewProjection;

    layout (location = 0) in vec3 a_vertex;
    layout (location = 1) in vec3 a_normal;

    out vec3 v_worldPosition;
    flat out vec3 v_normal;

    void main()
    {
        gl_Position = modelViewProjection * vec4(a_vertex, 1.0);

        v_worldPosition = (modelMatrix * vec4(a_vertex, 1.0)).xyz;
        v_normal = normalize((modelMatrix * vec4(a_normal, 0.0)).xyz);
    }
)";

static const char * s_fragmentShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require
    #extension GL_ARB_shading_language_include : require

    #define LIGHT_PROCESSING_PHONG

    #include </gloperate/shaders/lightProcessing.glsl>

    uniform samplerBuffer colorTypeData;
    uniform samplerBuffer positionData;
    uniform samplerBuffer attenuationData;

    uniform vec3 eye;
    uniform float glossiness;

    in vec3 v_worldPosition;
    flat in vec3 v_normal;

    layout (location = 0) out vec4 fragColor;

    void main()
    {
        vec3 color = lightIntensity(
            v_worldPosition,
            vec3(1.0),
            vec3(0.5),
            v_normal,
            glossiness,
            eye,
            colorTypeData,
            positionData,
            attenuationData);

        fragColor = vec4(color, 1.0);
    }
)";

static const glm::vec3 cameraEye(0.0f, -1.5f, -3.0f);


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(LightTestStage, gloperate::Stage)


LightTestStage::LightTestStage(Environment * environment, const std::string & name)
: Stage(environment, "LightTestStage", name)
, renderInterface(this)
, glossiness("glossiness", this, 0.0f)
, totalTime("totalTime", this, 0.0f)
, lightColorTypeData("lightColorTypeData", this, nullptr)
, lightPositionData("lightPositionData", this, nullptr)
, lightAttenuationData("lightAttenuationData", this, nullptr)
{
}

LightTestStage::~LightTestStage()
{
}

void LightTestStage::onContextInitialize(AbstractGLContext * /*context*/)
{
    // setup Geometry
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_cube, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    positionBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(1);
    normalBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    normalBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3));
    m_vao->enable(1);

    //TODO this is a memory leak! Use resource loader?
    // setup Program
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * fragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("colorTypeData", 0);
    m_program->setUniform("positionData", 1);
    m_program->setUniform("attenuationData", 2);

    m_program->setUniform("eye", cameraEye);

    auto dataFolderPath = dataPath();
    // [TODO]: fix memory leak
    globjects::NamedString::create("/gloperate/shaders/lightProcessing.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lightProcessing.glsl"));
    globjects::NamedString::create("/gloperate/shaders/lightProcessingDiffuse.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lightProcessingDiffuse.glsl"));
    globjects::NamedString::create("/gloperate/shaders/lightProcessingPhong.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lightProcessingPhong.glsl"));
}

void LightTestStage::onProcess(AbstractGLContext * context)
{
    if (!m_program)
        onContextInitialize(context);

    // Get viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Update transformation
    auto model = glm::rotate((*totalTime) / 3.0f, glm::vec3(0, 1, 0));
    auto view = glm::lookAt(cameraEye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projection = glm::perspective(30.0f, viewport.z / viewport.w, 0.1f, 10.0f);
    m_program->setUniform("modelMatrix", model);
    m_program->setUniform("modelViewProjection", projection * view * model);

    // Update other uniforms
    m_program->setUniform("glossiness", (*glossiness));

    // Bind textures
    if (*lightColorTypeData)
        (*lightColorTypeData)->bindActive(0);
    if (*lightPositionData)
        (*lightPositionData)->bindActive(1);
    if (*lightAttenuationData)
        (*lightAttenuationData)->bindActive(2);

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 14);
    m_program->release();

    // Unbind textures
    if (*lightColorTypeData)
        (*lightColorTypeData)->unbindActive(0);
    if (*lightPositionData)
        (*lightPositionData)->unbindActive(1);
    if (*lightAttenuationData)
        (*lightAttenuationData)->unbindActive(2);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}


} // namespace gloperate
