
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
    Q_PROPERTY(QString profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(int profileIndex WRITE setProfileIndex)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString format READ format)
    Q_PROPERTY(QString codec READ codec)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(int fps READ fps)
    Q_PROPERTY(int seconds READ seconds)
    Q_PROPERTY(int gopsize READ gopsize)
    Q_PROPERTY(int bitrate READ bitrate)


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
    *    Set profile index
    *
    *  @param[in] index
    *    Index of currently chosen profile
    */
    void setProfileIndex(int index);

    /**
    *  @brief
    *    Get title
    *
    *  @return
    *    Profile title (e.g. "AVI (codec: mpeg4)")
    */
    QString title() const;

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

    /**
    *  @brief
    *    Get width
    *
    *  @return
    *    Video width
    */
    int width() const;

    /**
    *  @brief
    *    Get height
    *
    *  @return
    *    Video height
    */
    int height() const;

    /**
    *  @brief
    *    Get fps
    *
    *  @return
    *    Video fps
    */
    int fps() const;

    /**
    *  @brief
    *    Get duration
    *
    *  @return
    *    Video duration (in seconds)
    */
    int seconds() const;

    /**
    *  @brief
    *    Get gop size
    *
    *  @return
    *    Size of pixel groups (gop size)
    */
    int gopsize() const;

        /**
    *  @brief
    *    Get bit rate
    *
    *  @return
    *    Video Bit rate
    */
    int bitrate() const;

signals:
    void profileChanged();
    void profileIndexChanged();


protected:
    /**
    *  @brief
    *    Load profile paths from profile directory
    */
    void initializeAvailableProfiles();

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
    QList<QString>     m_profilePaths;      ///< Available profiles in profile directory as paths
    QList<QString>     m_profileTitles;     ///< Available profiles in profile directory as titles
    QString            m_profileDirectory;  ///< Path to profile location
    QString            m_profile;           ///< Path to current profile file
    QString            m_title;             ///< Profile title
    QString            m_format;            ///< Video format
    QString            m_codec;             ///< Video codec
    int                m_width;             ///< Video width
    int                m_height;            ///< Video heigth
    int                m_fps;               ///< Video fps
    int                m_seconds;           ///< Video duration (in seconds)
    int                m_gopsize;           ///< Size of pixel groups
    int                m_bitrate;           ///< Video bitrate
};


} // namespace gloperate_qtquick
