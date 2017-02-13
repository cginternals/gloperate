
#pragma once


#include <gloperate-qt/base/Application.h>

/**
*  @brief
*    Costumized application instance for the gloperate-viewer.
*/
class Application : public gloperate_qt::Application
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    The gloperate environment
    *  @param[inout] argc
    *    The number of command line arguments
    *  @param[inout] argv
    *    The array of command line arguments
    *
    *  @remarks
    *    Both the number and contents of the arguments may get changed.
    *
    *  @see
    *     QApplication::QApplication(int &argc, char **argv, int)
    */
    Application(gloperate::Environment * environment, int & argc, char ** argv);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Application();
};
