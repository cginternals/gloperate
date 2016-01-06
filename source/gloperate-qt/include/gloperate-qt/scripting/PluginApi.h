
#pragma once


#include <vector>
#include <string>

#include <reflectionzeug/Object.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class PluginManager;
    class Plugin;
}


namespace gloperate_qt
{


/**
*  @brief
*    Plugin manager functions for scripting interface
*/
class GLOPERATE_QT_API PluginApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] pluginManager
    *    Plugin manager used by the viewer
    */
    PluginApi(gloperate::PluginManager * pluginManager);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PluginApi();

    // Plugin API
    reflectionzeug::Variant searchPaths();
    void setSearchPaths(const reflectionzeug::Variant & paths);
    void addSearchPath(const std::string & path);
    void removeSearchPath(const std::string & path);
    void scan(const std::string & identifier, bool reload);
    bool load(const std::string & filePath, bool reload);
    reflectionzeug::Variant plugins();
    reflectionzeug::Variant plugin(const std::string & name);


protected:
    reflectionzeug::Variant getPluginInfo(gloperate::Plugin * plugin) const;


protected:
    gloperate::PluginManager * m_pluginManager;  ///< Plugin manager used by the viewer
};


} // namespace gloperate_qt
