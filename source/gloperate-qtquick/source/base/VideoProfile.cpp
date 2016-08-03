
#include <gloperate-qtquick/base/VideoProfile.h>

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
, m_profile(m_profileDirectory + "/default.json")
{
    setProfile(m_profileDirectory + "/default.json");
}

VideoProfile::~VideoProfile()
{
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

    m_profile = profile;

    emit profileChanged();
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

bool VideoProfile::loadJsonProfile(QString profile)
{
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

    m_format = json.value("format").toString();
    m_codec = json.value("codec").toString();

    m_width = json.contains("width")     ? json.value("width").toInt()   : 800;
    m_height = json.contains("height")   ? json.value("height").toInt()  : 600;
    m_fps = json.contains("fps")         ? json.value("fps").toInt()     : 30;
    m_seconds = json.contains("seconds") ? json.value("seconds").toInt() : 5;
    m_gopsize = json.contains("gopsize") ? json.value("gopsize").toInt() : 0;

    return true;
}


} // namespace gloperate_qtquick
