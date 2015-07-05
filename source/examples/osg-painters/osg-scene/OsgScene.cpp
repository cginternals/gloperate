
#include "OsgScene.h"

#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>


using namespace gloperate;
using namespace gloperate_osg;


OsgScene::OsgScene(gloperate::ResourceManager & resourceManager)
: OsgPainter("osgScene", resourceManager)
{
}

OsgScene::~OsgScene()
{
}

void OsgScene::onInitialize()
{
    OsgPainter::onInitialize();

    setScene(osgDB::readNodeFile("data/osg-scene/cow.osg"));

    viewer()->setCameraManipulator(new osgGA::TrackballManipulator());

    osgViewer::StatsHandler * statsHandler = new osgViewer::StatsHandler;
    viewer()->addEventHandler(statsHandler);
}
