
#include <gloperate-qt/scripting/ViewerApi.h>

#include <gloperate-qt/viewer/Viewer.h>


namespace gloperate_qt {


ViewerApi::ViewerApi(Viewer * viewer)
: reflectionzeug::Object("viewer")
, m_viewer(viewer)
{
    // Register functions
    this->addFunction("loadPainterByName", this, &ViewerApi::loadPainterByName);
}

ViewerApi::~ViewerApi()
{
}

void ViewerApi::loadPainterByName(const std::string & name)
{
    assert(m_viewer);

    m_viewer->loadPainterByName(name);
}


} // namespace gloperate_qt
