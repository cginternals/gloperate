
#pragma once


#include <QApplication>

//#include <gloperate-qt/viewer/Application.h>


class Application : public QApplication
// public gloperate_qt::Application
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();
};
