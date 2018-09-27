
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage for blitting from one framebuffer to another
*/
template <typename T>
class VectorSelectionStage : public Stage
{
public:
    // Inputs
    Input<std::vector<T> *> elements;     ///< Vector containing all possible elements
    Input<int>              index;        ///< Target index of the selected element
    Input<T>                defaultValue; ///< Default value if index or vector is not valid

    // Outputs
    Output<T>               element;      ///< Selected element


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    VectorSelectionStage(Environment * environment, const std::string & name = "VectorSelectionStage");


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
};


} // namespace gloperate


#include <gloperate/stages/base/VectorSelectionStage.inl>
