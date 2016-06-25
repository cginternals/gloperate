
#pragma once


#include <QObject>
#include <QTimer>
#include <QTime>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class Environment;
}


namespace gloperate_qt
{


/**
*  @brief
*    Update manager that controls the Qt main loop for timing updates
*
*  @remarks
*    This class must be instanciated once for the gloperate timing
*    to work correctly. It maintains a Qt timer that is activated
*    whenever a timer in gloperate is active and calls environment.update()
*    to update the timing information in gloperate.
*/
class GLOPERATE_QT_API UpdateManager : public QObject
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the manager belongs (must NOT be null!)
    */
    UpdateManager(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    ~UpdateManager();

    /**
    *  @brief
    *    Trigger a timer update
    */
    void wakeTimer();


protected:
    /**
    *  @brief
    *    Called on timer update
    */
    void onTimer();


protected:
    gloperate::Environment * m_environment; ///< Gloperate environment
    QTimer                   m_timer;       ///< Timer for continuous update
    QTime                    m_time;        ///< Time measurement
};


} // namespace gloperate_qt
