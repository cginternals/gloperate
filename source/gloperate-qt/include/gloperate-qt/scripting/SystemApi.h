
#pragma once


#include <map>
#include <string>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <QString>
#include <gloperate/ext-includes-end.h>

#include <signalzeug/Signal.h>

#include <reflectionzeug/Object.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt {


class QtOpenGLWindow;


/**
*  @brief
*    System functions for scripting interface
*/
class GLOPERATE_QT_API SystemApi : public QObject, public reflectionzeug::Object
{
    Q_OBJECT


public:
    signalzeug::Signal<const std::string &> command;
    signalzeug::Signal<const std::string &> output;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] openGLWindow
    *    Pointer to the OpenGL window (can be nullptr)
    *
    *  @remarks
    *    If openGLWindow is set, the context of that window
    *    is made current before sending key press events
    *    to the scripting environment.
    */
    SystemApi(QtOpenGLWindow * openGLWindow);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~SystemApi();

    /**
    *  @brief
    *    Set help text
    *
    *  @param[in] text
    *    Help text that is displayed on help()
    */
    void setHelpText(const std::string & text);

    // Script helper functions
    void help();
    void print(const reflectionzeug::Variant & value);
    void load(const std::string & filename);

    // File functions
    std::string readFile(const std::string & filename);
    void writeFile(const std::string & filename, const reflectionzeug::Variant & value);
    void appendFile(const std::string & filename, const reflectionzeug::Variant & value);

    // Keyboard functions
    int  onKeyPress(const reflectionzeug::Variant & func);
    void clearKeyPress(int id);
    void clearAllKeyPress();

    // System functions
    void exit();


protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);
    void keyPressed(const QString & key);


protected:
    QtOpenGLWindow                                 * m_openGLWindow;      ///< Pointer to the OpenGL window (can be nullptr)
    std::map<int, reflectionzeug::AbstractFunction*> m_keypressCallbacks; ///< List of key-press callbacks
    std::string                                      m_helpText;          ///< Help text displyed on help()
};


} // namespace gloperate_qt
