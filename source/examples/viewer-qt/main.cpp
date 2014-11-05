
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate/resources/ResourceManager.h>

#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <gloperate-qt/QtTextureStorer.h>

#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QString>
#include <QMainWindow>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());
    resourceManager.addStorer(new QtTextureStorer());

    // Initialize plugin manager
    PluginManager::init(QCoreApplication::applicationFilePath().toStdString());

    // Load example plugins
    PluginManager pluginManager;
    pluginManager.addPath("plugins");
    pluginManager.scan("painters");

    // Choose a painter
	std::string name = (argc > 1) ? argv[1] : "CubeScape";
    std::cout << "Trying to create painter '" << name << "'\n";

    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin(name);

	if (!plugin)
	{
		globjects::fatal() << "Plugin '" << name << "' not found. Listing plugins found:";
		pluginManager.printPlugins();

		return 1;
	}

	painter = plugin->createPainter(resourceManager);

    // Create OpenGL window
    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

	QtOpenGLWindow * window = new QtOpenGLWindow(resourceManager, format);
	window->setPainter(painter);

	QRect rect = QApplication::desktop()->screenGeometry(); // used to center the mainwindow on desktop

    // Create main window
    QMainWindow mainWindow;
	mainWindow.setGeometry((rect.width() - 1280) / 2, (rect.height() - 720) / 2, 1280, 720);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(window));
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);

    mainWindow.show();

    return app.exec();
}
