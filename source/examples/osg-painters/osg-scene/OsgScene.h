
#pragma once

#include <gloperate-osg/OsgPainter.h>


class OsgScene : public gloperate_osg::OsgPainter
{
public:
    OsgScene(gloperate::ResourceManager & resourceManager, const std::string & relDataPath);
    virtual ~OsgScene();


protected:
    virtual void onInitialize() override;
};
