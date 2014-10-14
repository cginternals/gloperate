#pragma once


#include <gloperate/pipelines/Data.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate-osg/OsgRenderStage.h>


namespace osg {
    class Texture;
}


namespace gloperate_osg
{


class GLOPERATE_OSG_API OsgFboRenderStage : public OsgRenderStage
{


public:
    OsgFboRenderStage(const std::string & name);
    virtual ~OsgFboRenderStage();


protected:
    virtual void handleViewportChanged() override;
    virtual void updateFbo_osg();
    virtual void updateFbo();


public:
    gloperate::Data<globjects::ref_ptr<globjects::Texture> > m_colorTexture;
    gloperate::Data<globjects::ref_ptr<globjects::Texture> > m_depthTexture;


protected:
    unsigned int getOsgTextureId(const osg::Texture * texture) const;


protected:
    osg::Texture * m_colorTextureOsg;
    osg::Texture * m_depthTextureOsg;


};


} // namespace gloperate_osg
