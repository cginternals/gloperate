#include <QApplication>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "ShaderCompiler.h"


namespace
{

const auto applicationDescription = R"(
Compiles shaders and prints out compiler errors.

If a shader file is specified, this shader file will get compiled.
If a directory is specified, each file with the extension 'vert', 'tcs', 'tes', 'geom', 'frag, and 'comp' will get compiled.

The include directories may contain a alias each, separated with ':'.
The alias directory is the directory the OpenGL NamedString is registered with.

Example:
./build/gloperate-shader-compiler data/)";

}

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("gloperate-shader-compiler");
    QCoreApplication::setApplicationVersion("1.1");

    QCommandLineParser parser;
    parser.setApplicationDescription(applicationDescription);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument("configFile", "the configuration file");

    parser.process(app);
    
    const auto arguments = parser.positionalArguments();
    
    if (arguments.isEmpty())
        return -1;
    
    const auto configPath = arguments.first();
    
    if (!QFile::exists(configPath))
        return -1;
    
    QFile jsonFile{configPath};
    
    if (!jsonFile.open(QIODevice::ReadOnly))
        return -1;
    
    QJsonParseError error{};
    
    const auto jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &error);
    
    if (error.error != QJsonParseError::NoError)
        return -1;
    
    if (!jsonDocument.isObject())
        return -1;
    
    return ShaderCompiler::process(jsonDocument.object()) ? 0 : -1;
}
