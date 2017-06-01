
#include <gloperate-glkernel/stages/MultiFrameAggregationStage.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>


static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2( +1.f, -1.f ),
    glm::vec2( +1.f, +1.f ),
    glm::vec2( -1.f, -1.f ),
    glm::vec2( -1.f, +1.f ) } };

static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    layout (location = 0) in vec2 a_vertex;
    out vec2 v_uv;

    void main()
    {
        v_uv = a_vertex * 0.5 + 0.5;
        gl_Position = vec4(a_vertex, 0.0, 1.0);
    }
)";

static const char * s_fragmentShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform sampler2D tex;

    layout (location = 0) out vec4 fragColor;

    in vec2 v_uv;

    void main()
    {
        fragColor = texture(tex, v_uv);
    }
)";


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameAggregationStage, gloperate::Stage)


MultiFrameAggregationStage::MultiFrameAggregationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, aggregationFBO("aggregationFBO", this)
, texture("texture", this)
, textureRerendered("textureRerendered", this)
, viewport("viewport", this)
, aggregationFactor("aggregationFactor", this)
, aggregatedFBO("aggregatedFBO", this)
{
}

MultiFrameAggregationStage::~MultiFrameAggregationStage()
{
}

void MultiFrameAggregationStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    setupGeometry();
    setupProgram();
}

void MultiFrameAggregationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();
}

void MultiFrameAggregationStage::onProcess()
{
    if (!(*texture))
        return;

    gl::glViewport(
        0,              // Origin (0,0) because content was already shifted in main render pass
        0,              // Applying the origin again would shift the result again
        (*viewport).z,
        (*viewport).w
    );

    globjects::Framebuffer * fbo = *aggregationFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    (*texture)->bindActive(0);
    gl::glBlendColor(0.0f, 0.0f, 0.0f, *aggregationFactor);
    gl::glBlendFunc(gl::GL_CONSTANT_ALPHA, gl::GL_ONE_MINUS_CONSTANT_ALPHA);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glEnable(gl::GL_BLEND);
    gl::glDisable(gl::GL_DEPTH_TEST);

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    gl::glDisable(gl::GL_BLEND);
    gl::glEnable(gl::GL_DEPTH_TEST);

    aggregatedFBO.setValue(*aggregationFBO);
}

void MultiFrameAggregationStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void MultiFrameAggregationStage::setupProgram()
{
    //TODO this is a memory leak! Use resource loader?
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

    m_program->setUniform("tex", 0);
}


} // namespace gloperate_glkernel
