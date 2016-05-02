
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
    cppexpose::Variant searchPaths();
    void addSearchPath(const std::string & path);
    void removeSearchPath(const std::string & path);
    void scan(const std::string & identifier);


protected:
    ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null!)
};


} // namespace gloperate
