
#include <gloperate-qt/scripting/ViewerApi.h>

#include <gloperate-qt/viewer/Viewer.h>


namespace gloperate_qt {


ViewerApi::ViewerApi(Viewer * viewer)
: reflectionzeug::Object("viewer")
, m_viewer(viewer)
{
    // Register functions
    this->addFunction("loadPainter", this, &ViewerApi::loadPainter);
}

ViewerApi::~ViewerApi()
{
}

void ViewerApi::loadPainter(const std::string & name)
{
    assert(m_viewer);

    m_viewer->loadPainter(name);
}


} // namespace gloperate_qt
