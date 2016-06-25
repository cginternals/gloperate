
#include <gloperate/base/System.h>

#include <fstream>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


System::System(ViewerContext * viewerContext)
: cppexpose::Object("system")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("load",       this, &System::load);
    addFunction("readFile",   this, &System::readFile);
    addFunction("writeFile",  this, &System::writeFile);
    addFunction("appendFile", this, &System::appendFile);
    addFunction("exit",       this, &System::exit);
}

System::~System()
{
}

void System::load(const std::string & filename)
{
    std::string code = readFile(filename);

    if (!code.empty()) {
        // Execute command
        m_viewerContext->executeScript(code);
    }
}

std::string System::readFile(const std::string & filename)
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

void System::writeFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename);

    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void System::appendFile(const std::string & filename, const cppexpose::Variant & value)
{
    std::ofstream f(filename, std::ios_base::app);

    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

void System::exit(int exitCode)
{
    m_viewerContext->exit(exitCode);
}


} // namespace gloperate
