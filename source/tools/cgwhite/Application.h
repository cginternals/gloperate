#pragma once

#include <gloperate-qtapplication/AbstractApplication.h>


class Application : public gloperate_qtapplication::AbstractApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();
};
