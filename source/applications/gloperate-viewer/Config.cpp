
#include "Config.h"

#include <QSettings>
#include <QStringList>

#include <gloperate/base/Environment.h>


const QString SETTINGS_PLUGINS("Plugins");
const QString SETTINGS_STYLE("Style");


Config::Config(gloperate::Environment & environment)
: m_environment(environment)
, m_style("")
{
    // Initialize settings
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    // Read plugin paths
    QStringList paths = settings.value(SETTINGS_PLUGINS).toStringList();
    for (auto path : paths)
    {
        m_environment.componentManager()->addPluginPath(
            path.toStdString()
        );
    }

    // Read UI style
    m_style = settings.value(SETTINGS_STYLE).toString();
}

Config::~Config()
{
    QSettings settings;

    // Save plugin paths
    QStringList paths;
    for (auto path : m_environment.componentManager()->pluginPaths())
    {
        paths << QString::fromStdString(path);
    }
    settings.setValue(SETTINGS_PLUGINS, paths);
}

const QString & Config::style() const
{
    return m_style;
}

void Config::setStyle(const QString & style)
{
    m_style = style;

    QSettings settings;
    settings.setValue(SETTINGS_STYLE, m_style);
}
