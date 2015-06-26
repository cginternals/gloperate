
#pragma once


#include <gloperate-qt/viewer/AbstractApplication.h>


class Application : public gloperate_qt::AbstractApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();
};
