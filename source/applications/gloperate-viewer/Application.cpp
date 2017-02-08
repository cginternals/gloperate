
#include "Application.h"

#include <QFileInfo>

#include <gloperate/gloperate-version.h>


Application::Application(gloperate::Environment * environment, int & argc, char ** argv)
: gloperate_qt::Application(environment, argc, argv)
{
    const auto fi = QFileInfo(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());
    QApplication::setApplicationName(GLOPERATE_PROJECT_NAME);
    QApplication::setApplicationVersion(GLOPERATE_VERSION);
    QApplication::setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);

}

Application::~Application()
{
}
