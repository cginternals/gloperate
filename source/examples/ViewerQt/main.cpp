#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>


using namespace gloperate_qt;


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

//  gloperate::Painter * painter = new SimpleTexture();
    gloperate::Painter * painter = new RotatingQuad();

    QtOpenGLWindow * glWindow = new QtOpenGLWindow();
    glWindow->setPainter(painter);

    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    mainWindow.show();

    return app.exec();
}
