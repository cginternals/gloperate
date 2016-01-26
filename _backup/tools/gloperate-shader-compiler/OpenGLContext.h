#pragma once

#include <memory>


class QJsonObject;
class QSurfaceFormat;

class JsonParseError;


class OpenGLContext
{
public:
    static OpenGLContext fromJsonConfig(const QJsonObject & config, JsonParseError * error);

public:
    OpenGLContext();
    ~OpenGLContext();
    
    OpenGLContext(OpenGLContext && context);
    OpenGLContext & operator=(OpenGLContext && rhs);
    
    void setFormat(const QSurfaceFormat & format);

    bool create();

    bool makeCurrent();
    void doneCurrent();
    
private:
    class Private;
    std::unique_ptr<Private> m_p;
};
