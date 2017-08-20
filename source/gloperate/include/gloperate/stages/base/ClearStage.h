
#pragma once


#include <cppassist/typelist/TypeList.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Framebuffer;
    class FramebufferAttachment;
}


namespace gloperate
{


class AbstractClearInput;
class ClearValueAdder;


/**
*  @brief
*    Stage that clears the screen with a background color
*
*  If a valid viewport is set (width and height are greater or equal to '0', only the area of
*  the given viewport is cleared, otherwise the full render targets are cleared.
*/
class GLOPERATE_API ClearStage : public Stage
{
    friend class ClearValueAdder;


public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ClearStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that clears the screen with a background color"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )

    /**
    *  @brief
    *    The list of supported types for framebuffer clearing
    */
    using SupportedClearValueTypes = cppassist::TypeList<int, float, std::pair<float, int>, Color, glm::vec4, glm::ivec4, glm::uvec4>;


public:
    // Interfaces
    RenderInterface renderInterface; ///< Renderinterface to manage render targets inputs and outputs

    // Inputs
    Input<bool> clear; ///< Flag if buffers should get cleared


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
    ClearStage(Environment * environment, const std::string & name = "ClearStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ClearStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;
    virtual void onContextDeinit(AbstractGLContext * content) override;

    /**
    *  @brief
    *    Reprocess inputs and build up input helper structure for easy clear value and render target association
    */
    void reprocessInputs();


protected:
    bool                                             m_reprocessInputs; ///< Recreate input helper structure upon next process
    std::vector<std::unique_ptr<AbstractClearInput>> m_clearInputs;     ///< Clear values of differing types
};


} // namespace gloperate
