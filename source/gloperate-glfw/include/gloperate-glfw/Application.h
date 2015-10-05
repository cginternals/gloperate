
#pragma once


#include <string>

#include <gloperate-glfw/gloperate-glfw_api.h>


namespace gloperate_glfw
{


/**
*  @brief
*    Singleton representing the (main) window's message loop
*/
class GLOPERATE_GLFW_API Application
{
public:
    static const std::string & applicationFilePath();
    static const std::string & applicationPath();

    static void setApplicationName(const std::string & name);
    static const std::string & applicationName();

    static int exec();
    static void quit(int code = 0);


public:
    Application(int & argc, char ** argv);

    void start();
    void stop(int code = 0);

    int exitCode();
    bool isRunning() const;


protected:
    void pollEvents();
    void processEvents();


protected:
    static std::string baseName(const std::string & filePath);
    static std::string path(const std::string & filePath);


protected:
    static Application * s_app;


protected:
    int m_exitCode;
    bool m_running;

    std::string m_filePath;
    std::string m_path;
    std::string m_name;
};


} // namespace gloperate_glfw
