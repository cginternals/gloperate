
#include <QApplication>
#include <QQmlEngine>

#include <globjects/base/baselogging.h>

#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>

#include <reflectionzeug/Object.h>

#include <scriptzeug/ScriptContext.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/ScriptContext.h>


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


class TestApi : public reflectionzeug::Object
{
public:
    TestApi(const std::string & name, int value)
    : reflectionzeug::Object(name)
    , m_value(value)
    {
        addProperty<int>("value", this, &TestApi::value, &TestApi::setValue);

        addFunction("sayHello", this, &TestApi::sayHello);
    }

    ~TestApi()
    {
    }

    int value() const
    {
        globjects::info() << "value()";

        return m_value;
    }

    void setValue(int v)
    {
        globjects::info() << "setValue()";

        m_value = v;
    }

    void sayHello(const std::string & msg, int count)
    {
        int num = (count > 0) ? count : 1;

        for (int i=0; i<num; i++) {
            globjects::info() << msg;
        }
    }


protected:
    int m_value;
};


int main(int argc, char * argv[])
{
    // Initialize Qt application
    QApplication app(argc, argv);

    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create viewer context
    ViewerContext viewerContext;
    UpdateManager updateManager(&viewerContext);

    // Create QML engine
    QmlEngine qmlEngine(&viewerContext);
    qmlEngine.addImportPath(qmlPath);

    // Create scripting context
    scriptzeug::ScriptContext scriptContext(
        new gloperate_qtquick::ScriptContext(&scriptContext, &qmlEngine)
    );
    TestApi * test1 = new TestApi("test1", 42);
    TestApi * test2 = new TestApi("test2", 23);
    test1->addProperty(test2);
    scriptContext.registerObject(test1);

    // Load and show QML
    QuickView * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl(qmlPath + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    return app.exec();
}
