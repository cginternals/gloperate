
#pragma once


#include <cppexpose/reflection/Object.h>

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
*    Scripting API for accessing the component manager
*/
class GLOPERATE_API ComponentsApi : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    ComponentsApi(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~ComponentsApi();


protected:
    // Scripting functions
    cppexpose::Variant pluginPaths();
    void addPluginPath(const std::string & path);
    void removePluginPath(const std::string & path);
    void scanPlugins(const std::string & identifier);
    cppexpose::Variant components();
    void printComponents();


protected:
    ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null!)
};


} // namespace gloperate
