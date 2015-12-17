
#include "OsgScene.h"

#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include <iozeug/filename.h>

#include <gloperate/base/registernamedstrings.h>


using namespace gloperate;
using namespace gloperate_osg;


OsgScene::OsgScene(gloperate::ResourceManager & resourceManager, const reflectionzeug::Variant & pluginInfo)
: OsgPainter("osgScene", resourceManager, pluginInfo)
, m_dataPath("")
{
    // Get data path
    const reflectionzeug::VariantMap * map = pluginInfo.asMap();
    if (map && map->count("dataPath") > 0) {
        m_dataPath = map->at("dataPath").value<std::string>();
        m_dataPath = iozeug::ensurePathSeparatorEnding(m_dataPath);
    }
}

OsgScene::~OsgScene()
{
}

void OsgScene::onInitialize()
{
    OsgPainter::onInitialize();

    setScene(osgDB::readNodeFile(m_dataPath + "data/osg-scene/cow.osg"));

    viewer()->setCameraManipulator(new osgGA::TrackballManipulator());

    osgViewer::StatsHandler * statsHandler = new osgViewer::StatsHandler;
    viewer()->addEventHandler(statsHandler);
}
