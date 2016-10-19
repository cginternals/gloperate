
#include <gloperate-qtquick/VideoProfile.h>

#include <gloperate/gloperate.h>

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


namespace gloperate_qtquick
{


VideoProfile::VideoProfile(QQuickItem * parent)
: QQuickItem(parent)
, m_profileDirectory(QString::fromStdString(gloperate::dataPath()) + "/gloperate/video_profiles")
{
    initializeAvailableProfiles();
    
    if (m_profilePaths.size() > 0)
    {
        setProfileIndex(0);
    }
}

VideoProfile::~VideoProfile()
{
}

void VideoProfile::initializeAvailableProfiles()
{
    m_profilePaths = QStringList();
    m_profileTitles = QStringList();

    QDir profileDirecotry(m_profileDirectory);
    profileDirecotry.setNameFilters(QStringList("*.json"));

    if (!profileDirecotry.exists())
    {
        qWarning() << "Cannot find directory: " << m_profileDirectory;
        return;
    }

    for (auto entry : profileDirecotry.entryList())
    {
        if (!loadJsonProfile(m_profileDirectory + "/" + entry))
        {
            continue;
        }

        m_profilePaths << m_profile;
        m_profileTitles << m_title;
    }
}

QList<QString> VideoProfile::availableProfiles() const
{
    return m_profileTitles;
}

QString VideoProfile::profile() const
{
    return m_profile;
}

void VideoProfile::setProfile(QString profile)
{
    if (!loadJsonProfile(profile))
    {
        return;
    }

    emit profileChanged();
}

void VideoProfile::setProfileIndex(int index)
{
    setProfile(m_profilePaths[index]);
}

QString VideoProfile::title() const
{
    return m_title;
}

QString VideoProfile::format() const
{
    return m_format;
}

QString VideoProfile::codec() const
{
    return m_codec;
}

int VideoProfile::width() const
{
    return m_width;
}

int VideoProfile::height() const
{
    return m_height;
}

int VideoProfile::fps() const
{
    return m_fps;
}

int VideoProfile::seconds() const
{
    return m_seconds;
}

int VideoProfile::gopsize() const
{
    return m_gopsize;
}

int VideoProfile::bitrate() const
{
    return m_bitrate;
}

bool VideoProfile::loadJsonProfile(QString profile)
{
    m_profile = profile;

    QString val;
    QFile file(profile);
    if (!file.exists())
    {
        qWarning() << "Cannot find profile: " << profile;
        return false;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
    if (jsonDoc.isEmpty())
    {
        qWarning() << "The profile file appears not to be a valid (Qt-readable) json file.";
        return false;
    }

    QJsonObject json = jsonDoc.object();

    if (!json.contains("format") || !json.contains("codec"))
    {
        qWarning() << "Video profile must provide 'format' and 'codec' field.";
        return false;
    }

    // Mandatory fields
    m_format = json.value("format").toString();
    m_codec = json.value("codec").toString();

    // Optional fields. Adjustable in GUI afterwards
    m_width = json.contains("width")     ? json.value("width").toInt()   : 800;
    m_height = json.contains("height")   ? json.value("height").toInt()  : 600;
    m_fps = json.contains("fps")         ? json.value("fps").toInt()     : 30;
    m_seconds = json.contains("seconds") ? json.value("seconds").toInt() : 5;

    // Optional fields. NOT adjustable in GUI afterwards
    m_title = json.contains("title")     ? json.value("title").toString() : m_format;
    m_gopsize = json.contains("gopsize") ? json.value("gopsize").toInt()  : 0;
    m_bitrate = json.contains("bitrate") ? json.value("bitrate").toInt()  : 0;

    return true;
}


} // namespace gloperate_qtquick
