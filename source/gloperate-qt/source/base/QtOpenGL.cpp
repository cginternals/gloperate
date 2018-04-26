
#include <gloperate-qt/base/QtOpenGL.h>

#include <QOpenGLContext>



namespace gloperate_qt
{


glbinding::ProcAddress QtOpenGL::getProcAddress(QOpenGLContext * context, const char * name)
{
    // Return if name is invalid
    if (name == nullptr)
    {
        return nullptr;
    }

    // Convert name to QString
    const auto symbol = std::string(name);
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        const auto qtSymbol = QByteArray::fromStdString(symbol);
    #else
        const auto qtSymbol = QByteArray::fromRawData(symbol.c_str(), symbol.size());
    #endif

    // Lookup proc address
    return context->getProcAddress(qtSymbol);
}


} // namespace gloperate_qt
