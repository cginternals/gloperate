
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


class Environment;


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
    *  @param[in] environment
    *    Environment (must NOT be null!)
    */
    ComponentManager(Environment * environment);

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
    Environment * m_environment; ///< Gloperate environment (must NOT be null!)
};


} // namespace gloperate
