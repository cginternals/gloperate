
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
    Q_PROPERTY(QString profile READ profile WRITE setProfile)
    Q_PROPERTY(QList<QString> profiles READ availableProfiles)


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
    *    Get available profiles
    *
    *  @return
    *    List of paths to available profiles
    */
    QList<QString> availableProfiles() const;


protected:
    QString        m_profileDirectory;  ///< Path to profile location
    QString        m_profile;           ///< Path to current profile file
    QList<QString> m_availableProfiles; ///< Available profile files in profile directory
};


} // namespace gloperate_qtquick
