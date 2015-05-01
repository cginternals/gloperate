
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

    gloperate_qtapplication::Viewer viewer;
    viewer.show();

    return app.exec();
}
