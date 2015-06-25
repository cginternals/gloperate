
#include <gloperate-qt/viewer/AbstractApplication.h>

#include <QApplication>
#include <QStringList>

#include <widgetzeug/MessageHandler.h>


namespace gloperate_qt
{

AbstractApplication::AbstractApplication(int & argc, char ** argv)
: QApplication(argc, argv)
{
	qInstallMessageHandler(widgetzeug::globalMessageHandler);
}

AbstractApplication::~AbstractApplication()
{
}

} // namespace gloperate_qt
