
#include <gloperate/scripting/ComponentsApi.h>

#include <fstream>

#include <cppexpose/variant/Variant.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


ComponentsApi::ComponentsApi(ViewerContext * viewerContext)
: cppexpose::Object("components")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("searchPaths",      this, &ComponentsApi::searchPaths);
    addFunction("addSearchPath",    this, &ComponentsApi::addSearchPath);
    addFunction("removeSearchPath", this, &ComponentsApi::removeSearchPath);
    addFunction("scan",             this, &ComponentsApi::scan);
}

ComponentsApi::~ComponentsApi()
{
}

cppexpose::Variant ComponentsApi::searchPaths()
{
    std::vector<std::string> paths = m_viewerContext->componentManager()->searchPaths();

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto path : paths) {
        lst.asArray()->push_back(cppexpose::Variant(path));
    }

    return lst;
}

void ComponentsApi::addSearchPath(const std::string & path)
{
    m_viewerContext->componentManager()->addSearchPath(path);
}

void ComponentsApi::removeSearchPath(const std::string & path)
{
    m_viewerContext->componentManager()->removeSearchPath(path);
}

void ComponentsApi::scan(const std::string & identifier)
{
    m_viewerContext->componentManager()->scan(identifier);
}


} // namespace gloperate
