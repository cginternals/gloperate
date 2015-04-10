
#include <gloperate-qtapplication/AbstractApplication.h>

#include <QApplication>
#include <QStringList>

#include <widgetzeug/MessageHandler.h>


namespace gloperate_qtapplication
{

AbstractApplication::AbstractApplication(int & argc, char ** argv)
: QApplication(argc, argv)
{
	qInstallMessageHandler(widgetzeug::globalMessageHandler);
}

AbstractApplication::~AbstractApplication()
{
}

} // namespace gloperate_qtapplication
