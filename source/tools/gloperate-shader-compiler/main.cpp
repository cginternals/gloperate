
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include <QMainWindow>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include <gloperate-qt/viewer/QtOpenGLWindowBase.h>

#include "ShaderCompiler.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("gloperate-shader-compiler");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Compiles shaders and prints out compiler errors");
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument("shaderFile", "The shader file to compile", "<shader file>");
    parser.addPositionalArgument("includePath", "The shader include path", "[include paths...]");

    parser.process(app);

    if (parser.positionalArguments().isEmpty())
    {
        qDebug() << "No shader file specified";

        return 3;
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
            return 2;
        } else {
            QSurfaceFormat intermediateFormat = context.format();
            //qDebug().nospace() << "Created intermediate OpenGL context " << intermediateFormat.version().first << "." << intermediateFormat.version().second;

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
        return 2;
    } else {
        //qDebug().nospace() << "Created OpenGL context " << m_context->format().version().first << "." << m_context->format().version().second;
    }

    context.makeCurrent(&window);

    ShaderCompiler compiler(parser.positionalArguments().mid(1));

    auto success = compiler.compile(parser.positionalArguments().front());

    context.doneCurrent();

    return success ? 0 : 1;
}
