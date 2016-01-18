
// [TODO] This belongs into the cmake platform files!!
#if defined(WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup")
#endif


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <widgetzeug/dark_fusion_style.h>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/viewer/Viewer.h>

#include "Application.h"


int main(int argc, char * argv[])
{
    Application app(argc, argv);

    widgetzeug::enableDarkFusionStyle();

    gloperate_qt::Viewer viewer;
    viewer.show();

    std::string painterName = (argc > 1) ? argv[1] : "Logo";
    viewer.loadPainter(painterName);

    return app.exec();
}
