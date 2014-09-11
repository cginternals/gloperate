#include <iostream>
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <QScopedPointer>
#include <QDockWidget>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <extended-examples/ExtendedCubeScape/ExtendedCubeScape.h>

#include <reflectionzeug/PropertyGroup.h>

#include <propertyguizeug/PropertyBrowser.h>

#include <gloperate/capabilities/VirtualTimeCapability.h>
#include <gloperate/ChronoTimer.h>

#include <gloperate-qt/TimePropagator.h>

using namespace gloperate;
using namespace gloperate_qt;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Create plugin manager
    PluginManager pluginManager;
    pluginManager.scan("examples");
    for (Plugin * plugin : pluginManager.plugins()) {
        std::cout << "Plugin '" << plugin->name() << "' (" << plugin->type() << ")\n";
        std::cout << "  version " << plugin->version() << "\n";
        std::cout << "  by " << plugin->vendor() << "\n";
        std::cout << "  " << plugin->description() << "\n";
        std::cout << "\n";
    }

    // Create resource manager
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());

    // Choose a painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin("ExtendedCubeScape");
    if (plugin) {
        painter = plugin->createPainter();
    } else {
//      painter = new SimpleTexture();
        painter = new ExtendedCubeScape(&resourceManager);
    }

    QScopedPointer<TimePropagator> mainloop(nullptr);

    // Create OpenGL window
    QtOpenGLWindow * glWindow = new QtOpenGLWindow();
    if (painter) {
        glWindow->setPainter(painter);

        if (painter->supports<gloperate::VirtualTimeCapability>())
        {
            mainloop.reset(new TimePropagator(glWindow, painter->getCapability<gloperate::VirtualTimeCapability>()));
        }
    }

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    reflectionzeug::PropertyGroup * properties = dynamic_cast<reflectionzeug::PropertyGroup *>(painter);

    if (properties != nullptr)
    {
        QDockWidget * dock = new QDockWidget(&mainWindow);

        dock->setWidget(new propertyguizeug::PropertyBrowser(properties, dock));

        mainWindow.addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}

