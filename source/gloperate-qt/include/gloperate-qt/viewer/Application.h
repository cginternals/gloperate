
#pragma once

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API Application : public QApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();
};


} // namespace gloperate_qt
