
#pragma once


#include <vector>
#include <string>

#include <reflectionzeug/Object.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class PluginManager;
}


namespace gloperate_qt {


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
    reflectionzeug::Variant list();


protected:
    gloperate::PluginManager * m_pluginManager;  ///< Plugin manager used by the viewer
};


} // namespace gloperate_qt
