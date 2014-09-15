#pragma once


#include <gloperate-osg/OsgPainter.h>
#include <osg-examples/osg_examples_api.h>


class OSG_EXAMPLES_API OsgSimpleScene : public gloperate_osg::OsgPainter
{


public:
    OsgSimpleScene(gloperate::ResourceManager & resourceManager);
    virtual ~OsgSimpleScene();


protected:
    virtual void onInitialize() override;


};
