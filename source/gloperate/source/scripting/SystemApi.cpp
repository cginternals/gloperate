
#include <gloperate/scripting/SystemApi.h>

#include <fstream>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


SystemApi::SystemApi(ViewerContext * viewerContext)
: cppexpose::Object("system")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("load",       this, &SystemApi::load);
    addFunction("readFile",   this, &SystemApi::readFile);
    addFunction("writeFile",  this, &SystemApi::writeFile);
    addFunction("appendFile", this, &SystemApi::appendFile);
    addFunction("exit",       this, &SystemApi::exit);
}

SystemApi::~SystemApi()
{
}

void SystemApi::load(const std::string & filename)
{
    std::string code = readFile(filename);
    if (!code.empty()) {
        // Execute command
        m_viewerContext->scriptEnvironment()->execute(code);
    }
}

std::string SystemApi::readFile(const std::string & filename)
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

void SystemApi::writeFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename);
    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void SystemApi::appendFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename, std::ios_base::app);
    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void SystemApi::exit(int exitCode)
{
    m_viewerContext->exit(exitCode);
}


} // namespace gloperate
