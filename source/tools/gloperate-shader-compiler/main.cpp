
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include <QMainWindow>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include <gloperate-qt/viewer/QtOpenGLWindowBase.h>

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

enum ShaderCompilerResult
{
    Success = 0,
    CompilationError,
    ContextCreationError,
    ShaderFileMissing
};

}

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("gloperate-shader-compiler");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(applicationDescription);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument("shaderFile", "The shader file or directory to compile", "<shader file or directory>");
    parser.addPositionalArgument("includePath", "The shader include path", "[include paths...]");

    parser.process(app);

    if (parser.positionalArguments().isEmpty())
    {
        qDebug() << "No shader file specified";

        return ShaderFileMissing;
    }

    QWindow window;
    QOpenGLContext context;

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    #ifndef NDEBUG
        format.setOption(QSurfaceFormat::DebugContext);
    #endif

    window.setSurfaceType(QWindow::OpenGLSurface);
    window.create();

    if (format.version().first < 3)
    {
        context.setFormat(format);
        if (!context.create()) {
            qDebug() << "Could not create intermediate OpenGL context.";

            return ContextCreationError;
        } else {
            QSurfaceFormat intermediateFormat = context.format();

            if ((intermediateFormat.version().first == 3 && intermediateFormat.version().second == 0) || intermediateFormat.version().first < 3)
            {
                format.setMajorVersion(3);
                format.setMinorVersion(2);
                format.setProfile(QSurfaceFormat::CoreProfile);
            }
        }
    }

    context.setFormat(format);
    if (!context.create()) {
        qDebug() << "Could not create intermediate OpenGL context.";

        return ContextCreationError;
    }

    context.makeCurrent(&window);

    ShaderCompiler compiler(parser.positionalArguments().mid(1));

    auto success = compiler.compile(parser.positionalArguments().front());

    context.doneCurrent();

    return success ? Success : CompilationError;
}
