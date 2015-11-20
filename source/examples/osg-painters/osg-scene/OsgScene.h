
#pragma once

#include <gloperate-osg/OsgPainter.h>


class OsgScene : public gloperate_osg::OsgPainter
{
public:
    OsgScene(gloperate::ResourceManager & resourceManager, const reflectionzeug::Variant & pluginInfo);
    virtual ~OsgScene();


protected:
    virtual void onInitialize() override;

protected:
    /* Data path */
    std::string m_dataPath;
};
