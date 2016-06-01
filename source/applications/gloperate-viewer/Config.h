
#pragma once


#include <QObject>

#include <QString>


namespace gloperate {
    class ViewerContext;
}


class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString style READ style WRITE setStyle)

public:
    Config(gloperate::ViewerContext & viewerContext);
    virtual ~Config();

    const QString & style() const;
    void setStyle(const QString & style);


protected:
    gloperate::ViewerContext & m_viewerContext; ///< Main gloperate context
    QString                    m_style;         ///< Currently selected style
};
