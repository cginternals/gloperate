
#include <gloperate-qtquick/base/VideoProfile.h>

#include <gloperate/gloperate.h>

#include <QDir>
#include <QDebug>


namespace gloperate_qtquick
{


VideoProfile::VideoProfile(QQuickItem * parent)
: QQuickItem(parent)
, m_profileDirectory(QString::fromStdString(gloperate::dataPath()) + "/gloperate/video_profiles")
, m_profile(m_profileDirectory + "/default.json")
{
    setProfile(m_profileDirectory + "/default.json");

    availableProfiles();
}

VideoProfile::~VideoProfile()
{
}

QString VideoProfile::profile() const
{
    return m_profile;
}

void VideoProfile::setProfile(QString profile)
{
    m_profile = profile;
}

QList<QString> VideoProfile::availableProfiles() const
{
    QStringList profiles;

    QDir profileDirecotry(m_profileDirectory);
    profileDirecotry.setNameFilters(QStringList("*.json"));

    if (!profileDirecotry.exists())
    {
        qWarning() << "Cannot find directory: " << m_profileDirectory;
        return profiles;
    }

    for (auto entry : profileDirecotry.entryList())
    {
        profiles << m_profileDirectory + "/" + entry;
    }

    return profiles;
}


} // namespace gloperate_qtquick
