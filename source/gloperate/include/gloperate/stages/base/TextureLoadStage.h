
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Parameter.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that loads a texture from a file
*/
class GLOPERATE_API TextureLoadStage : public Stage
{
    CPPEXPOSE_DECLARE_COMPONENT(TextureLoadStage, gloperate::Stage)


public:
    // Parameters
    Parameter<std::string>       filename; ///< Texture filename

    // Outputs
    Output<globjects::Texture *> texture;  ///< Texture object


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    TextureLoadStage(ViewerContext * viewerContext, const std::string & name = "TextureLoadStage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureLoadStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void loadTexture();


protected:
    // Data
    globjects::ref_ptr<globjects::Texture> m_texture; ///< Texture
};


} // namespace gloperate
