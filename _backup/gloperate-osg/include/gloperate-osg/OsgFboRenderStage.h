
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/pipeline/Data.h>

#include <gloperate-osg/OsgRenderStage.h>


namespace osg 
{
    class Texture;
}


namespace gloperate_osg
{


/**
*  @brief
*    Rendering stage that renders an OSG scene into an FBO
*
*  This rendering stage sets up an FBO in OSG to render a scene into
*  a set of textures. Afterwards, these texture are converted into
*  globjects textures to be used in a gloperate rendering pipeline.
*  Please note that a valid viewport has to be supplied as input to
*  this stage (m_viewport) in order to correctly setup the FBO
*  and textures.
*/
class GLOPERATE_OSG_API OsgFboRenderStage : public OsgRenderStage
{
public:
    // Output data
    gloperate::Data<globjects::ref_ptr<globjects::Texture> > m_colorTexture;
    gloperate::Data<globjects::ref_ptr<globjects::Texture> > m_depthTexture;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Name of the stage
    */
    OsgFboRenderStage(const std::string & name);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgFboRenderStage();


protected:
    // Virtual OsgRenderStage functions
    virtual void handleViewportChanged() override;
    virtual void postOsgRendering();

    /**
    *  @brief
    *    Called to setup target FBO for rendering the OSG scene
    */
    virtual void updateFbo_osg();

    /**
    *  @brief
    *    Called to setup textures for use with gloperate pipeline
    */
    virtual void updateFbo();


protected:
    /**
    *  @brief
    *    Get texture ID of a given OSG texture
    */
    unsigned int getOsgTextureId(const osg::Texture * texture) const;


protected:
    osg::Texture * m_colorTextureOsg;
    osg::Texture * m_depthTextureOsg;
};


} // namespace gloperate_osg
