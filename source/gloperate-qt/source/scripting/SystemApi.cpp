
#include <gloperate-qt/scripting/SystemApi.h>

#include <fstream>

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <QKeyEvent>
#include <QString>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/viewer/Viewer.h>
#include <gloperate-qt/viewer/QtOpenGLWindow.h>


namespace gloperate_qt {


SystemApi::SystemApi(QtOpenGLWindow * openGLWindow)
: reflectionzeug::Object("system")
, m_openGLWindow(openGLWindow)
{
    // Register functions
    this->addFunction("help",             this, &SystemApi::help);
    this->addFunction("print",            this, &SystemApi::print);
    this->addFunction("load",             this, &SystemApi::load);
    this->addFunction("readFile",         this, &SystemApi::readFile);
    this->addFunction("writeFile",        this, &SystemApi::writeFile);
    this->addFunction("onKeyPress",       this, &SystemApi::onKeyPress);
    this->addFunction("clearKeyPress",    this, &SystemApi::clearKeyPress);
    this->addFunction("clearAllKeyPress", this, &SystemApi::clearAllKeyPress);
    this->addFunction("exit",             this, &SystemApi::exit);
}

SystemApi::~SystemApi()
{
    // Destroy keyboard callbacks
    for (std::map<int, reflectionzeug::AbstractFunction*>::iterator it = m_keypressCallbacks.begin(); it != m_keypressCallbacks.end(); ++it) {
        reflectionzeug::AbstractFunction *func = it->second;
        delete func;
    }
}

void SystemApi::setHelpText(const std::string & text)
{
    m_helpText = text;
}

void SystemApi::help()
{
    this->output(m_helpText);
}

void SystemApi::print(const reflectionzeug::Variant & value)
{
    this->output(value.toJSON());
}

void SystemApi::load(const std::string & filename)
{
    std::string code = readFile(filename);
    if (!code.empty()) {
        // Execute command
        this->command(code);
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

void SystemApi::writeFile(const std::string & filename, const reflectionzeug::Variant & value)
{
    std::ofstream f(filename);
    if (f.is_open()) {
        f << value.value<std::string>();
        f.close();
    }
}

int SystemApi::onKeyPress(const reflectionzeug::Variant & func)
{
    static int nextID = 0;
    nextID++;

    // Check if a function has been passed
    if (func.canConvert<reflectionzeug::AbstractFunction*>()) {
        // Get callback function and make a persistent copy
        reflectionzeug::AbstractFunction * function = func.value<reflectionzeug::AbstractFunction*>()->clone();

        // Add function to list of callbacks
        m_keypressCallbacks.insert(std::pair<int, reflectionzeug::AbstractFunction*>(nextID, function));

        // Return ID
        return nextID;
    }

    // Invalid
    return -1;
}

void SystemApi::clearKeyPress(int id)
{
    // Get callback with given ID
    auto it = m_keypressCallbacks.find(id);
    if (it != m_keypressCallbacks.end()) {
        // Delete callback function
        reflectionzeug::AbstractFunction * function = it->second;
        if (function) {
            delete function;
        }

        // Remove from list
        m_keypressCallbacks.erase(it);
    }
}

void SystemApi::clearAllKeyPress()
{
    // Destroy keyboard callbacks
    for (std::map<int, reflectionzeug::AbstractFunction*>::iterator it = m_keypressCallbacks.begin(); it != m_keypressCallbacks.end(); ++it) {
        reflectionzeug::AbstractFunction * func = it->second;
        delete func;
    }
    m_keypressCallbacks.clear();
}

void SystemApi::exit()
{
    QApplication::exit(0);
}

bool SystemApi::eventFilter(QObject * obj, QEvent * event)
{
    // Filter key events
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);
        keyPressed(QString() + (char)keyEvent->key());
    }

    // Pass events on to default event processing
    return QObject::eventFilter(obj, event);
}

void SystemApi::keyPressed(const QString & key)
{
    // Make sure the OpenGL context is current
    if (m_openGLWindow) {
        m_openGLWindow->makeCurrent();
    }

    // For each connected callback function
    for (std::map<int, reflectionzeug::AbstractFunction*>::iterator it = m_keypressCallbacks.begin(); it != m_keypressCallbacks.end(); ++it) {
        // Get function
        reflectionzeug::AbstractFunction * func = it->second;

        // Pass key value to function
        std::vector<reflectionzeug::Variant> params;
        reflectionzeug::Variant k(key.toStdString());
        params.push_back(k);

        // Invoke function
        reflectionzeug::Variant res = func->call(params);
    }
}


} // namespace gloperate_qt
