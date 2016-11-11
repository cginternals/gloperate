
#include <gloperate-qt/viewer/Application.h>

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <QStringList>
#include <QCommandLineParser>
#include <QSettings>
#include <gloperate/ext-includes-end.h>

#include <widgetzeug/MessageHandler.h>


namespace gloperate_qt
{


namespace
{
const QString ARGNAME_CONFIG{ "config" };
}


Application * Application::self = nullptr;


Application::Application(int & argc, char ** argv)
: QApplication(argc, argv)
{
    self = this;

	qInstallMessageHandler(widgetzeug::globalMessageHandler);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({ ARGNAME_CONFIG, "Path to the configuration file", "config-path" });

    parser.process(*this);

    // defaults to "" if not set
    m_settingsFilePath = parser.value(ARGNAME_CONFIG);
}


Application::~Application()
{
}


std::unique_ptr<QSettings> Application::settings()
{
    if (self->m_settingsFilePath.isEmpty())
    {
        return std::unique_ptr<QSettings>{new QSettings};
    }
    return std::unique_ptr<QSettings>{new QSettings{self->m_settingsFilePath, QSettings::IniFormat}};
}


} // namespace gloperate_qt
