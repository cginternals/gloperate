
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that creates a procedural texture
*/
class GLOPERATE_API ProceduralTextureStage : public Stage
{
    CPPEXPOSE_DECLARE_COMPONENT(ProceduralTextureStage, gloperate::Stage)


public:
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
    ProceduralTextureStage(ViewerContext * viewerContext, const std::string & name = "ProceduralTextureStage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProceduralTextureStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void createTexture();


protected:
    // Data
    globjects::ref_ptr<globjects::Texture> m_texture; ///< Texture
};


} // namespace gloperate
