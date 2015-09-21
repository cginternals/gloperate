#pragma once

#include <memory>


class QJsonObject;
class QSurfaceFormat;

class OpenGLContext
{
public:
    static OpenGLContext fromJsonConfig(const QJsonObject & config, bool * ok);

public:
    OpenGLContext();
    ~OpenGLContext();
    
    OpenGLContext(OpenGLContext && context);
    OpenGLContext & operator=(OpenGLContext && rhs);
    
    void setFormat(const QSurfaceFormat & format);

    bool create();

    void makeCurrent();
    void doneCurrent();
    
private:
    class Private;
    std::unique_ptr<Private> m_p;
};
