
#pragma once


#include <gloperate-qt/viewer/Application.h>


class Application : public gloperate_qt::Application
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();
};
