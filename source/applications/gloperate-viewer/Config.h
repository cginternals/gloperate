
#pragma once


#include <QObject>

#include <QString>


namespace gloperate {
    class Environment;
}


class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString style READ style WRITE setStyle)

public:
    Config(gloperate::Environment & environment);
    virtual ~Config();

    const QString & style() const;
    void setStyle(const QString & style);


protected:
    gloperate::Environment & m_environment; ///< Main gloperate environment
    QString                  m_style;       ///< Currently selected style
};
