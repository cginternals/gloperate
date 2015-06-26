
#pragma once

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API AbstractApplication : public QApplication
{
public:
    AbstractApplication(int & argc, char ** argv);
    virtual ~AbstractApplication();
};


} // namespace gloperate_qt
