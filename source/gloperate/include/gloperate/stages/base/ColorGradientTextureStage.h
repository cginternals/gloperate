
#pragma once

#include <string>
#include <vector>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/ColorGradientList.h>


namespace gloperate
{

class GLOPERATE_API ColorGradientTextureStage : public gloperate::Stage
{
public:
    ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name = "ColorGradientTextureStage");

public:
    Input<ColorGradientList> gradients;
    Input<size_t> textureWidth;

    Output<globjects::ref_ptr<globjects::Texture>> gradientTexture;

protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
};

} // namespace gloperate
