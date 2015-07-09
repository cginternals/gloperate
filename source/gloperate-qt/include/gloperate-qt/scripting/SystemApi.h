
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


class Viewer;


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
    *  @param[in] viewer
    *    Pointer to the main viewer window
    */
    SystemApi(Viewer * viewer);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~SystemApi();

    // Script helper functions
    void load(const std::string & filename);
    void print(const reflectionzeug::Variant & value);

    // File functions
    std::string readFile(const std::string & filename);
    void writeFile(const std::string & filename, const reflectionzeug::Variant & value);

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
    Viewer                                         * m_viewer;            ///< Pointer to the main viewer window
    std::map<int, reflectionzeug::AbstractFunction*> m_keypressCallbacks; ///< List of key-press callbacks
};


} // namespace gloperate_qt
