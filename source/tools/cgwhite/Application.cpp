#include "Application.h"


#include <QFileInfo>

#include "gloperate-version.h"


Application::Application(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
{
	const QFileInfo fi(QCoreApplication::applicationFilePath());

	QApplication::setApplicationDisplayName(fi.baseName());

    QApplication::setApplicationName(GLOPERATE_PROJECT_NAME);
    QApplication::setApplicationVersion(GLOPERATE_VERSION);

    QApplication::setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);
}

Application::~Application()
{
}
