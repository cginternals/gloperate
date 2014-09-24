#include <osg-examples/OsgSimpleScene/OsgSimpleScene.h>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>


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

    // Load a simple scene
    setScene(osgDB::readNodeFile("data/OsgSimpleScene/cow.osg"));

    // Create camera manipulator
    viewer()->setCameraManipulator(new osgGA::TrackballManipulator());

    // Add stats handler
    osgViewer::StatsHandler * statsHandler = new osgViewer::StatsHandler;
    viewer()->addEventHandler(statsHandler);
}
