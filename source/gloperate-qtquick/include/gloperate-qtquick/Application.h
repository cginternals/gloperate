
#pragma once


#include <QGuiApplication>
#include <QString>
#include <QTimer>

#include <gloperate/base/Environment.h>

#include <gloperate-qtquick/QmlEngine.h>


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt application object that initializes gloperate automatically
*
*  @remarks
*    At the beginning of your application, call Application::initialize().
*    This is important to initialize e.g. high DPI scaling and message
*    redirection. Then, instanciate the Application and load a Qml file
*    using loadQml(), or access the Qml engine via qmlEngine().
*    Call Application::exec() to run the main loop.
*/
class GLOPERATE_QTQUICK_API Application : public QGuiApplication
{
public:
    /**
    *  @brief
    *    Initialize QmlToolbox
    *
    *  @remarks
    *    This must be called BEFORE creating an instance of Application!
    */
    static void initialize();


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] argc
    *    Number of command line arguments
    *  @param[in] argv
    *    Command line arguments
    */
    Application(int & argc, char ** argv);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Application();

    /**
    *  @brief
    *    Get path to gloperate qml module
    *
    *  @return
    *    Path to gloperate qml module
    */
    const QString & gloperateModulePath() const;

    /**
    *  @brief
    *    Load QML file
    *
    *  @param[in] path
    *    Local path
    */
    void loadQml(const QString & path);

    /**
    *  @brief
    *    Get QML engine
    *
    *  @return
    *    QML engine
    */
    const QmlEngine & qmlEngine() const;

    /**
    *  @brief
    *    Get QML engine
    *
    *  @return
    *    QML engine
    */
    QmlEngine & qmlEngine();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment
    */
    const gloperate::Environment & environment() const;

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment
    */
    gloperate::Environment & environment();


protected:
    void onTimer();


protected:
    gloperate::Environment m_environment; ///< Main gloperate environment
    QmlEngine              m_qmlEngine;   ///< Spezialied QML engine for gloperate
    QTimer                 m_timer;       ///< Global timer (e.g., to update scripting timers)
};


} // namespace gloperate_qtquick
