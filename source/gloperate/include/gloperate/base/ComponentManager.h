
#pragma once


#include <cppexpose/reflection/Object.h>
#include <cppexpose/plugin/ComponentManager.h>

#include <gloperate/gloperate_api.h>


namespace cppexpose
{
    class Variant;
}


namespace gloperate
{


class ViewerContext;


/**
*  @brief
*    Component manager with script bindings
*/
class GLOPERATE_API ComponentManager : public cppexpose::Object, public cppexpose::ComponentManager
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    ComponentManager(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~ComponentManager();


protected:
    // Scripting functions
    cppexpose::Variant script_pluginPaths();
    void script_addPluginPath(const std::string & path);
    void script_removePluginPath(const std::string & path);
    void script_scanPlugins(const std::string & identifier);
    cppexpose::Variant script_components();
    void script_printComponents();


protected:
    ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null!)
};


} // namespace gloperate
