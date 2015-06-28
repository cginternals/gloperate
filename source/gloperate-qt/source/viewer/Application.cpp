
#include <gloperate-qt/viewer/Application.h>

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <QStringList>
#include <gloperate/ext-includes-end.h>

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
