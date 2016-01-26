
#pragma once


#include <string>
#include <memory>
#include <list>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace reflectionzeug {
    class Object;
}

namespace scriptzeug {
    class ScriptContext;
}

namespace widgetzeug {
    class ScriptPromptWidget;
}


namespace gloperate_qt
{


class QtOpenGLWindow;
class SystemApi;
class TimerApi;


/**
*  @brief
*    Scripting environment that handles a scripting context, scripting APIs, etc.
*/
class GLOPERATE_QT_API ScriptEnvironment
{
public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] openGLWindow
    *    Open GL canvas (can be nullptr)
    *
    *  @notes
    *    - If openGLWindow is set, key presses from that window are promoted to the system API
    *    - If scriptPrompt is set, it is used to automatically register auto-complete keywords
    */
    ScriptEnvironment(QtOpenGLWindow * openGLWindow = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    ~ScriptEnvironment();
    //@}

    //@{
    /**
    *  @brief
    *    Get scripting context
    *
    *  @return
    *    Script context
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
    *    This adds an reflectionzeug::Object to the scripting
    *    context. The name of the object is made available as a
    *    global variable to access the functions of the object.
    */
    void addScriptApi(reflectionzeug::Object * api);

    /**
    *  @brief
    *    Disconnect scripting API
    *
    *  @param[in] api
    *    Scripting API
    */
    void removeScriptApi(reflectionzeug::Object * api);

    /**
    *  @brief
    *    Connect scripting console
    *
    *  @param[in] console
    *    Scripting console widget (must be valid)
    */
    void connectScriptingConsole(widgetzeug::ScriptPromptWidget * console);

    /**
    *  @brief
    *    Execute script command
    *
    *  @param[in] command
    *    Command
    *
    *  @return
    *    Return value from the scripting command
    */
    reflectionzeug::Variant execute(const std::string & command);

    /**
    *  @brief
    *    Set help text
    *
    *  @param[in] text
    *    Help text that is displayed on help()
    */
    void setHelpText(const std::string & text);
    //@}


protected:
    std::unique_ptr<scriptzeug::ScriptContext> m_scriptContext; ///< JavaScript scripting context
    std::unique_ptr<SystemApi>                 m_systemApi;     ///< System function (files, print)
    std::unique_ptr<TimerApi>                  m_timerApi;      ///< Timer functions
    std::list<reflectionzeug::Object *>        m_apis;          ///< List of connected APIs
    QtOpenGLWindow                           * m_openGLWindow;  ///< OpenGL canvas (can be nullptr)
    widgetzeug::ScriptPromptWidget           * m_scriptPrompt;  ///< Script prompt widget (can be nullptr)
};


} // namespace gloperate_qt
