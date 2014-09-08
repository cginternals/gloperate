#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Create resource manager
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());

    // Choose a painter
//  gloperate::Painter * painter = new SimpleTexture();
    gloperate::Painter * painter = new RotatingQuad(&resourceManager);

    // Create OpenGL window
    QtOpenGLWindow * glWindow = new QtOpenGLWindow();
    glWindow->setPainter(painter);

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}
