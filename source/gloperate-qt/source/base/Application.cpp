
#include <gloperate-qt/base/Application.h>

#include <QApplication>

#include <gloperate/base/Environment.h>


namespace gloperate_qt
{


Application::Application(gloperate::Environment * environment, int & argc, char ** argv)
: QApplication(argc, argv)
{
    // Connect to exit-signal
    environment->exitApplication.connect([] (int exitCode)
    {
        QApplication::exit(exitCode);
    });
}

Application::~Application()
{
}


} // namespace gloperate_qt
