
#include <gloperate/base/System.h>

#include <fstream>

#include <cppassist/logging/logging.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


System::System(Environment * environment)
: cppexpose::Object("system")
, m_environment(environment)
{
    // Register functions
    addFunction("logLevel",    this, &System::scr_logLevel);
    addFunction("setLogLevel", this, &System::scr_setLogLevel);
    addFunction("safeMode",    this, &System::scr_safeMode);
    addFunction("load",        this, &System::scr_load);
    addFunction("readFile",    this, &System::scr_readFile);
    addFunction("writeFile",   this, &System::scr_writeFile);
    addFunction("appendFile",  this, &System::scr_appendFile);
    addFunction("exit",        this, &System::scr_exit);
}

System::~System()
{
}

int System::scr_logLevel()
{
    return cppassist::verbosityLevel();
}

void System::scr_setLogLevel(int logLevel)
{
    if (logLevel >= 0)
    {
        cppassist::setVerbosityLevel(logLevel);
    }
}

bool System::scr_safeMode()
{
    return m_environment->safeMode();
}

void System::scr_load(const std::string & filename)
{
    std::string code = scr_readFile(filename);

    if (!code.empty()) {
        // Execute command
        m_environment->executeScript(code);
    }
}

std::string System::scr_readFile(const std::string & filename)
{
    std::string txt;

    std::ifstream f(filename);
    if (f.is_open()) {
        std::string line;
        while (getline(f, line)) {
            txt = txt + line + "\n";
        }
        f.close();
    }

    return txt;
}

void System::scr_writeFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename);

    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void System::scr_appendFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename, std::ios_base::app);

    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void System::scr_exit(int exitCode)
{
    m_environment->exit(exitCode);
}


} // namespace gloperate
