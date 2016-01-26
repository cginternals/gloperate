
#include <gloperate-qt/scripting/ViewerApi.h>

#include <gloperate-qt/viewer/Viewer.h>


namespace gloperate_qt {


ViewerApi::ViewerApi(Viewer * viewer)
: reflectionzeug::Object("viewer")
, m_viewer(viewer)
{
    // Register functions
    this->addFunction("loadPainter",    this, &ViewerApi::loadPainter);
    this->addFunction("makeScreenshot", this, &ViewerApi::makeScreenshot);
}

ViewerApi::~ViewerApi()
{
}

void ViewerApi::loadPainter(const std::string & name)
{
    assert(m_viewer);

    m_viewer->loadPainter(name);
}

void ViewerApi::makeScreenshot(const std::string & filename, int width, int height, int frames)
{
    assert(m_viewer);

    m_viewer->makeScreenshot(filename, std::max(1, width), std::max(1, height), std::max(1, frames));
}


} // namespace gloperate_qt
