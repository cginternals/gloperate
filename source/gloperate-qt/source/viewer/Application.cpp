
#include <gloperate-qt/viewer/Application.h>

#include <QApplication>

#include <widgetzeug/MessageHandler.h>


namespace gloperate_qt
{


Application::Application(int & argc, char ** argv)
: QApplication(argc, argv)
{
    qInstallMessageHandler(widgetzeug::globalMessageHandler);
}

Application::~Application()
{
}


} // namespace gloperate_qt
