
#include "Config.h"

#include <QSettings>
#include <QStringList>

#include <gloperate/base/Environment.h>


namespace
{
    const QString settingsPlugins("Plugins");
    const QString settingsStyle("Style");
}


Config::Config(gloperate::Environment & environment)
: m_environment(environment)
, m_style("")
{
    // Initialize settings
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    // Read plugin paths
    QStringList paths = settings.value(settingsPlugins).toStringList();

    for (const auto & path : paths)
    {
        m_environment.componentManager()->addPluginPath(
            path.toStdString()
        );
    }

    // Read UI style
    m_style = settings.value(settingsStyle).toString();
}

Config::~Config()
{
    QSettings settings;

    // Save plugin paths
    QStringList paths;
    for (const auto & path : m_environment.componentManager()->pluginPaths())
    {
        paths << QString::fromStdString(path);
    }

    settings.setValue(settingsPlugins, paths);
}

const QString & Config::style() const
{
    return m_style;
}

void Config::setStyle(const QString & style)
{
    m_style = style;

    QSettings settings;
    settings.setValue(settingsStyle, m_style);
}
