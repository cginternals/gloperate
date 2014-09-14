#include <osg-examples/OsgSimpleScene/OsgSimpleScene.h>
#include <osgDB/ReadFile>


using namespace gloperate;
using namespace gloperate_osg;


OsgSimpleScene::OsgSimpleScene(gloperate::ResourceManager & resourceManager)
: OsgPainter(resourceManager)
{
}

OsgSimpleScene::~OsgSimpleScene()
{
}

void OsgSimpleScene::onInitialize()
{
	OsgPainter::onInitialize();

	setScene(osgDB::readNodeFile("data/OsgSimpleScene/cow.osg"));
}
