#pragma once


class QJsonObject;
class QSurfaceFormat;

class ShaderCompiler
{
public:
    static bool process(const QJsonObject & config);
    
    static QSurfaceFormat parseOpenGLFormat(
        const QJsonObject & config,
        bool & ok);
};
