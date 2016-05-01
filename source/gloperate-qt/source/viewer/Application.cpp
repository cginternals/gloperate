
#include <gloperate-qt/viewer/Application.h>

#include <QApplication>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate_qt
{


Application::Application(gloperate::ViewerContext * viewerContext, int & argc, char ** argv)
: QApplication(argc, argv)
{
    // Connect to exit-signal
    viewerContext->exitApplication.connect([] (int exitCode)
    {
        QApplication::exit(exitCode);
    });
}

Application::~Application()
{
}


} // namespace gloperate_qt
