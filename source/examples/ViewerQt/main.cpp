#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <gloperate-qt/qt-includes-end.h>
#include "ExampleWindow.h"


using namespace gloperate_qt;


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ExampleWindow * glWindow = new ExampleWindow();

    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    mainWindow.show();

    return app.exec();
}
