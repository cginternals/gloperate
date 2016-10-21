
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
class Texture;
} // namespace globjects


namespace gloperate
{


class GLOPERATE_API LightTestStage : public Stage
{
public:
    RenderInterface renderInterface;

    Input<float> glossiness;
    Input<float> totalTime;

    Input<globjects::Texture *> lightColorTypeData;
    Input<globjects::Texture *> lightPositionData;
    Input<globjects::Texture *> lightAttenuationData;

public:
    LightTestStage(Environment * environment, const std::string & name = "LightTestStage");
    virtual ~LightTestStage();

protected:
    virtual void onContextInitialize(AbstractGLContext * context);
    virtual void onProcess(AbstractGLContext * context);

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
    globjects::ref_ptr<globjects::Program>     m_program;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader;
};


}
