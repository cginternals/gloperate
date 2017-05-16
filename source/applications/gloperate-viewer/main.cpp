
#include <gloperate/gloperate-version.h>

#include <gloperate-qtquick/Application.h>


using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Initialize gloperate and qmltoolbox
    Application::initialize();

    // Initialize Qt application
    Application app(argc, argv);
    app.setApplicationName(GLOPERATE_PROJECT_NAME);
    app.setApplicationVersion(GLOPERATE_VERSION);
    app.setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    app.setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);

    // Load and show QML
    app.loadQml(app.gloperateModulePath() + "/Viewer.qml");

    // Run main loop
    return app.exec();
}
