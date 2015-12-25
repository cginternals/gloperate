#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class ColorGradientList;

class GLOPERATE_API ColorGradientPreparationStage : public gloperate::AbstractStage
{
public:
    ColorGradientPreparationStage();
    virtual ~ColorGradientPreparationStage();

public:
    InputSlot<ColorGradientList> gradients;
    InputSlot<std::pair<std::uint32_t, std::uint32_t>> pixmapSize;

    Data<std::vector<std::string>> names;
    Data<std::vector<std::vector<unsigned char>>> pixmaps;

protected:
    virtual void process() override;
};

} // namespace gloperate
