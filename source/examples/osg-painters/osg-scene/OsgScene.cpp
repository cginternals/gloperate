
#include "OsgScene.h"

#include <cpplocate/ModuleInfo.h>

#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include <iozeug/FilePath.h>

#include <gloperate/base/registernamedstrings.h>


using namespace gloperate;
using namespace gloperate_osg;


OsgScene::OsgScene(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: OsgPainter("osgScene", resourceManager, moduleInfo)
, m_dataPath("")
{
    // Get data path
    m_dataPath = moduleInfo.value("dataPath");
    m_dataPath = iozeug::FilePath(m_dataPath).path();
    if (m_dataPath.size() > 0) m_dataPath = m_dataPath + "/";
    else                       m_dataPath = "data/";
}

OsgScene::~OsgScene()
{
}

void OsgScene::onInitialize()
{
    OsgPainter::onInitialize();

    setScene(osgDB::readNodeFile(m_dataPath + "osg-scene/cow.osg"));

    viewer()->setCameraManipulator(new osgGA::TrackballManipulator());

    osgViewer::StatsHandler * statsHandler = new osgViewer::StatsHandler;
    viewer()->addEventHandler(statsHandler);
}
