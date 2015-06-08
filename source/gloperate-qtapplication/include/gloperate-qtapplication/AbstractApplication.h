#pragma once

#include <QApplication>

#include <gloperate-qtapplication/gloperate-qtapplication_api.h>


namespace gloperate_qtapplication
{

/** \brief ToDo
*
* Installs a Message Handler for extended logging capabilities.
*
* \code{.cpp}
* Moep
* \endcode
*/

class GLOPERATE_QTAPPLICATION_API AbstractApplication : public QApplication
{
public:
    AbstractApplication(int & argc, char ** argv);
    virtual ~AbstractApplication();
};

} // namespace gloperate_qtapplication
