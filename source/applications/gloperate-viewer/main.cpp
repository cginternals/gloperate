
#include <iostream>

#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QQmlEngine>
#include <QQmlContext>

#include <cppassist/string/conversion.h>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/scripting/ScriptContext.h>
#include <cppexpose/reflection/Property.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/scripting/ScriptEnvironment.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/QmlScriptContext.h>

#include "Config.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


class TreeNode : public cppexpose::Object
{
public:
    // Properties
    cppexpose::Property<int> id;


public:
    TreeNode(const std::string & name, cppexpose::PropertyGroup * parent = nullptr);
    virtual ~TreeNode();

    int getId() const;
    void setId(const int & id);

    void expand();
    void collaps();

    void print(const std::string & indent = "");


protected:
    int        m_id;
    TreeNode * m_left;
    TreeNode * m_right;
};

using namespace cppexpose;


static int g_nextId = 1;

TreeNode::TreeNode(const std::string & name, PropertyGroup * parent)
: Object(name, parent)
, id("id", this, this, &TreeNode::getId, &TreeNode::setId)
, m_id(g_nextId++)
, m_left(nullptr)
, m_right(nullptr)
{
    // Register functions
    addFunction("expand",  this, &TreeNode::expand);
    addFunction("collaps", this, &TreeNode::collaps);
    addFunction("print",   this, &TreeNode::print);
}

TreeNode::~TreeNode()
{
    std::cout << "TreeNode destroyed." << std::endl;
}

int TreeNode::getId() const
{
    return m_id;
}

void TreeNode::setId(const int & id)
{
    m_id = id;
}

void TreeNode::expand()
{
    // Abort if already expanded
    if (m_left || m_right)
    {
        return;
    }

    // Create child nodes
    m_left = new TreeNode("left");
    addProperty(m_left, PropertyOwnership::Parent);

    m_right = new TreeNode("right");
    addProperty(m_right, PropertyOwnership::Parent);
}

void TreeNode::collaps()
{
    // Destroy child nodes
    if (m_left)
    {
        destroyProperty(m_left);
        m_left = nullptr;
    }

    if (m_right)
    {
        destroyProperty(m_right);
        m_right = nullptr;
    }
}

void TreeNode::print(const std::string & indent)
{
    std::cout << "node(" << cppassist::toString<int>(m_id) << ")" << std::endl;

    if (m_left)
    {
        std::cout << indent << "  left: ";
        m_left ->print(indent + "  ");
    }

    if (m_right)
    {
        std::cout << indent << "  right: ";
        m_right->print(indent + "  ");
    }
}


int main(int argc, char * argv[])
{
    // Determine data paths
    const auto qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create viewer context
    ViewerContext viewerContext;

    // Initialize Qt application
    gloperate_qt::Application app(&viewerContext, argc, argv);
    const auto fi = QFileInfo(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());
    QApplication::setApplicationName(GLOPERATE_PROJECT_NAME);
    QApplication::setApplicationVersion(GLOPERATE_VERSION);
    QApplication::setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);

    // Load configuration
    Config config(viewerContext);

    // Configure update manager
    UpdateManager updateManager(&viewerContext);

    // Create QML engine
    QmlEngine qmlEngine(&viewerContext);
    qmlEngine.addImportPath(qmlPath);
    qmlEngine.rootContext()->setContextProperty("config", &config);

    // Create scripting context backend
    viewerContext.scriptEnvironment()->setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

    TreeNode root("root");
    viewerContext.scriptEnvironment()->scriptContext()->registerObject(&root);

    // Configure and load plugins
    viewerContext.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    viewerContext.componentManager()->scanPlugins("loaders");
    viewerContext.componentManager()->scanPlugins("stages");

    // Load and show QML
    auto * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl::fromLocalFile(qmlPath + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    int res = app.exec();

    // Clean up
    delete window;
    return res;
}
