
#pragma once


#include <string>
#include <memory>
#include <list>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate/gloperate_api.h>


namespace reflectionzeug {
    class Object;
}

namespace scriptzeug {
    class ScriptContext;
    class AbstractScriptContext;
}


namespace gloperate
{


class ViewerContext;
class SystemApi;
class TimerApi;


/**
*  @brief
*    Scripting environment that handles a scripting context, scripting APIs, etc.
*/
class GLOPERATE_API ScriptEnvironment
{
public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    ScriptEnvironment(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~ScriptEnvironment();

    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backendName
    *    Name of scripting backend to use (default: 'javascript')
    */
    void setupScripting(const std::string & backendName);

    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backend
    *    Scripting backend to use (must NOT be null)
    */
    void setupScripting(scriptzeug::AbstractScriptContext * backend);
    //@}

    //@{
    /**
    *  @brief
    *    Get scripting context
    *
    *  @return
    *    Script context (can be null)
    */
    const scriptzeug::ScriptContext * scriptContext() const;
    scriptzeug::ScriptContext * scriptContext();
    //@}

    //@{
    /**
    *  @brief
    *    Connect scripting API
    *
    *  @param[in] api
    *    Scripting API
    *
    *  @remarks
    *    This adds an reflectionzeug::Object to the scripting context.
    *    It is made available via the global object 'gloperate',
    *    e.g., 'gloperate.test' if the object's name is 'test'.
    */
    void addApi(reflectionzeug::Object * api);

    /**
    *  @brief
    *    Disconnect scripting API
    *
    *  @param[in] api
    *    Scripting API
    */
    void removeApi(reflectionzeug::Object * api);

    /**
    *  @brief
    *    Set help text
    *
    *  @param[in] text
    *    Help text that is displayed on help()
    */
    void setHelpText(const std::string & text);

    /**
    *  @brief
    *    Execute script
    *
    *  @param[in] code
    *    Script code
    *
    *  @return
    *    Return value from the scripting context
    */
    reflectionzeug::Variant execute(const std::string & code);
    //@}


protected:
    /**
    *  @brief
    *    Initialize scripting environment
    */
    void initialize();


protected:
    ViewerContext                              * m_viewerContext; ///< Viewer context (must NOT be null!)
    std::unique_ptr<scriptzeug::ScriptContext>   m_scriptContext; ///< JavaScript scripting context
    std::unique_ptr<SystemApi>                   m_systemApi;     ///< System function (files, print)
    std::unique_ptr<TimerApi>                    m_timerApi;      ///< Timer functions
    std::list<reflectionzeug::Object *>          m_apis;          ///< List of connected APIs
    std::string                                  m_helpText;      ///< Text that is displayed on 'help'
};


} // namespace gloperate
