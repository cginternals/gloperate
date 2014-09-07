#pragma once


#include <gloperate-glfw/gloperate-glfw_api.h>


namespace gloperate_glfw
{


class GLOPERATE_GLFW_API MainLoop
{


public:
    /** This enters the (main) windows message loop.
    */
    static int run();
    static void quit(int code = 0);


public:
    void start();
    void stop(int code = 0);
    int exitCode();
    bool isRunning() const;


protected:
    MainLoop();

    void pollEvents();
    void processEvents();


protected:
    int m_exitCode;
    bool m_running;


protected:
    static MainLoop s_mainLoop;


};


} // namespace gloperate_glfw
