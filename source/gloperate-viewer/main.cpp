
#include <memory>

//#include <gloperate-qt/viewer/Viewer.h>

#include "Application.h"


int main(int argc, char * argv[])
{
    Application app(argc, argv);

    /*
    gloperate_qt::Viewer viewer;
    viewer.show();

    std::string painterName = (argc > 1) ? argv[1] : "Logo";
    viewer.loadPainter(painterName);
    */

    return app.exec();
}
