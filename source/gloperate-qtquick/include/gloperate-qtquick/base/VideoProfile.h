
#pragma once


#include <QQuickItem>


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick video profile
*/
class VideoProfile : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> profiles READ availableProfiles)
    Q_PROPERTY(QString profile READ profile WRITE setProfile)
    Q_PROPERTY(QString format READ format)
    Q_PROPERTY(QString codec READ codec)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent item (can be null)
    */
    VideoProfile(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VideoProfile();

    /**
    *  @brief
    *    Get available profiles
    *
    *  @return
    *    List of paths to available profiles
    */
    QList<QString> availableProfiles() const;

    /**
    *  @brief
    *    Get profile
    *
    *  @return
    *    Path to profile file
    */
    QString profile() const;

    /**
    *  @brief
    *    Set profile
    *
    *  @param[in] profile
    *    Path to profile file
    */
    void setProfile(QString profile);

    /**
    *  @brief
    *    Get format
    *
    *  @return
    *    Video format (e.g. 'avi')
    */
    QString format() const;

    /**
    *  @brief
    *    Get codec
    *
    *  @return
    *    Video codec (e.g. 'codec')
    */
    QString codec() const;


protected:
    /**
    *  @brief
    *    Load a json profile from file
    *
    *  @param[in] profile
    *    Path to profile file
    *  @return
    *    True if loading the profile was successful, false otherwise
    */
    bool loadJsonProfile(QString profile);


protected:
    QList<QString> m_availableProfiles; ///< Available profile files in profile directory
    QString        m_profileDirectory;  ///< Path to profile location
    QString        m_profile;           ///< Path to current profile file
    QString        m_format;            ///< Video format
    QString        m_codec;             ///< Video codec
};


} // namespace gloperate_qtquick
