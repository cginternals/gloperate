
#pragma once


#include <QApplication>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate {
    class ViewerContext;
}


namespace gloperate_qt
{


/**
*  @brief
*    Qt application for gloperate
*
*  @remarks
*    Handles necessary initialization for gloperate applications.
*/
class GLOPERATE_QT_API Application : public QApplication
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    *  @param[in] argc
    *    Argument count
    *  @param[in] argv
    *    Argument list
    */
    Application(gloperate::ViewerContext * viewerContext, int & argc, char ** argv);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Application();
};


} // namespace gloperate_qt
