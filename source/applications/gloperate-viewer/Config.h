
#pragma once


#include <QObject>

#include <QString>


namespace gloperate
{
    class Environment;
} // namespace gloperate


/**
* @brief
*   The configuration of the gloperate-viewer application
*
*   Currently includes the following configuration parameters:
*    * style
*/
class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString style READ style WRITE setStyle)

public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    The gloperate environment
    */
    Config(gloperate::Environment & environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Config();

    /**
    *  @brief
    *    Get the style identifier
    *
    *  @return
    *    The style identifier
    */
    const QString & style() const;

    /**
     *  @brief
     *    Set the style identifier
     *
     *  @param[in] style
     *    The style identifier
     */
    void setStyle(const QString & style);


protected:
    gloperate::Environment & m_environment; ///< Main gloperate environment
    QString                  m_style;       ///< Identifier for currently selected style
};
