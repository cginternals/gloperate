#pragma once

#include <string>
#include <vector>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class ColorGradientList;

class GLOPERATE_API ColorGradientTextureStage : public gloperate::AbstractStage
{
public:
    ColorGradientTextureStage();
    virtual ~ColorGradientTextureStage();

public:
    InputSlot<ColorGradientList> gradients;
    InputSlot<size_t> textureWidth;

    Data<globjects::ref_ptr<globjects::Texture>> gradientTexture;

protected:
    virtual void process() override;
};

} // namespace gloperate
