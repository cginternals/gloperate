#include <QApplication>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "ShaderCompiler.h"


namespace
{

const auto applicationDescription = R"(
Compiles shaders and prints out compiler errors.)";

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
    {
        qDebug() << "No configuration file specified.";
        return 1;
    }
    
    const auto configPath = arguments.first();
    
    if (!QFile::exists(configPath))
    {
        qDebug() << "Configuration file could not be found.";
        return 1;
    }
    
    QFile jsonFile{configPath};
    
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Configurationa file could not be opened.";
        return 1;
    }
    
    QJsonParseError error{};
    
    const auto jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), &error);
    
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "Error parsing document:" << error.errorString();
        return 1;
    }
    
    return ShaderCompiler::process(jsonDocument) ? 0 : 1;
}
