
#pragma once


#include <vector>
#include <string>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/base/ComponentManager.h>
#include <gloperate/base/ResourceManager.h>
#include <gloperate/base/System.h>
#include <gloperate/base/TimerManager.h>
#include <gloperate/input/InputManager.h>


namespace cppexpose
{
    class ScriptContext;
    class AbstractScriptBackend;
    class Variant;
}


namespace gloperate
{


class Canvas;


/**
*  @brief
*    Main object for gloperate containing the top-level components needed for rendering.
*
*    The gloperate environment holds all necessary components used for the rendering
*    process, such as the plugin manager, input manager, resource loaders, etc. It
*    should usually be instantiated once by the top-level viewer application and
*    passed down to other objects that need access to these components. Some components
*    have to be provided by the windowing backend on creation of the environment.
*
*    The gloperate environment is independent from windows and OpenGL contexts and
*    should be initialized before creating actual viewers and their OpenGL contexts.
*/
class GLOPERATE_API Environment : public cppexpose::Object
{
    friend class Canvas;


public:
    cppexpose::Signal<int> exitApplication;   ///< Called when application shall exit


public:
    //@{
    /**
    *  @brief
    *    Constructor
    */
    Environment();

    /**
    *  @brief
    *    Destructor
    */
    ~Environment();
    //@}

    //@{
    /**
    *  @brief
    *    Get component manager
    *
    *  @return
    *    Component manager (must NOT be null)
    */
    const ComponentManager * componentManager() const;
    ComponentManager * componentManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get resource manager
    *
    *  @return
    *    Resource manager (must NOT be null)
    */
    const ResourceManager * resourceManager() const;
    ResourceManager * resourceManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get input manager
    *
    *  @return
    *    Input manager (must NOT be null)
    */
    const InputManager * inputManager() const;
    InputManager * inputManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get timer manager
    *
    *  @return
    *    Timer manager (never null)
    */
    const TimerManager * timerManager() const;
    TimerManager * timerManager();
    //@}

    //@{
    /**
    *  @brief
    *    Get canvases
    *
    *  @return
    *    List of registered canvases
    */
    const std::vector<Canvas *> & canvases() const;
    std::vector<Canvas *> canvases();
    //@}

    //@{
    /**
    *  @brief
    *    Get scripting context
    *
    *  @return
    *    Script context (can be null)
    */
    const cppexpose::ScriptContext * scriptContext() const;
    cppexpose::ScriptContext * scriptContext();
    //@}

    //@{
    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backendName
    *    Name of scripting backend to use (default: 'javascript')
    */
    void setupScripting(const std::string & backendName = "");

    /**
    *  @brief
    *    Setup scripting environment
    *
    *  @param[in] backend
    *    Scripting backend to use (must NOT be null)
    */
    void setupScripting(std::unique_ptr<cppexpose::AbstractScriptBackend> &&backend);

    /**
    *  @brief
    *    Execute script code
    *
    *  @param[in] code
    *    Script code
    *
    *  @return
    *    Return value from the scripting context
    */
    cppexpose::Variant executeScript(const std::string & code);

    /**
    *  @brief
    *    Exit application
    *
    *    This function emits the signal 'exitApplication'.
    *    Windowing backends are expected to connect to that signal
    *    and exit the application when receiving the signal.
    *
    *  @param[in] exitCode
    *    Exit code (default: 0)
    */
    void exit(int exitCode = 0);
    
    /**
    *  @brief
    *    Get safemode-flag
    *
    *  @return
    *    'true' if safemode is activated, else 'false'
    *
    *  @remarks
    *    If safemode is on, the gloperate viewer does not load its settings
    *    from file. Instead, it resets all settings to default and restores
    *    the config file.
    */
    bool safeMode() const;

    /**
    *  @brief
    *    Set safemode-flag
    *
    *  @param[in] safeMode
    *    'true' if safemode is activated, else 'false'
    *
    *  @remarks
    *    see safeMode()
    */
    void setSafeMode(bool safeMode);
    //@}


protected:
    //@{
    /**
    *  @brief
    *    Register local plugins (contained in gloperate itself)
    */
    void registerLocalPlugins();

    /**
    *  @brief
    *    Initialize scripting environment
    *
    *  @param[in] scriptContext
    *    New script context (must NOT be nullptr!)
    *
    *  @remarks
    *    If there is a script context already active, it will be deleted
    *    before the new one is initialized.
    */
    void initializeScripting(std::unique_ptr<cppexpose::ScriptContext> && scriptContext);

    /**
    *  @brief
    *    Register canvas
    *
    *  @param[in] canvas
    *    Canvas (must NOT be null!)
    */
    void registerCanvas(Canvas * canvas);

    /**
    *  @brief
    *    Un-register canvas
    *
    *  @param[in] canvas
    *    Canvas (must NOT be null!)
    */
    void unregisterCanvas(Canvas * canvas);
    //@}


protected:
    ComponentManager                          m_componentManager; ///< Manager for plugin libraries and components
    ResourceManager                           m_resourceManager;  ///< Resource manager for loaders/storers
    System                                    m_system;           ///< System functions for scripting
    InputManager                              m_inputManager;     ///< Manager for Devices, -Providers and InputEvents
    TimerManager                              m_timerManager;     ///< Manager for scripting timers

    std::vector<Canvas *>                     m_canvases;         ///< List of active canvases

    std::unique_ptr<cppexpose::ScriptContext> m_scriptContext;    ///< Scripting context

    std::string                               m_helpText;         ///< Text that is displayed on 'help'
    bool                                      m_safeMode;         ///< If 'true', settings are not loaded from file but reset to default values
};


} // namespace gloperate
