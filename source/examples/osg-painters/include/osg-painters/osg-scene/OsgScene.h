#pragma once

#include <gloperate-osg/OsgPainter.h>

#include <osg-painters/osg_painters_api.h>


class OSG_PAINTERS_API OsgScene : public gloperate_osg::OsgPainter
{
public:
    OsgScene(gloperate::ResourceManager & resourceManager);
    virtual ~OsgScene();

protected:
    virtual void onInitialize() override;
};
