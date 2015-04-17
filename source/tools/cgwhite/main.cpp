
#if defined(WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup")
#endif


#include "Application.h"

#include <memory>

#include <gloperate-qtapplication/Viewer.h>

#include <widgetzeug/dark_fusion_style.hpp>


int main(int argc, char * argv[])
{
    Application app(argc, argv);

    widgetzeug::enableDarkFusionStyle();

    std::unique_ptr<gloperate_qtapplication::Viewer> viewer(new gloperate_qtapplication::Viewer()); // make_unique is C++14
    viewer->show();

    return app.exec();
}
