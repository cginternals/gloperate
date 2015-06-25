#pragma once

#include <QApplication>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{

/** \brief ToDo
*
* Installs a Message Handler for extended logging capabilities.
*
* \code{.cpp}
* Moep
* \endcode
*/

class GLOPERATE_QT_API AbstractApplication : public QApplication
{
public:
    AbstractApplication(int & argc, char ** argv);
    virtual ~AbstractApplication();
};

} // namespace gloperate_qt
