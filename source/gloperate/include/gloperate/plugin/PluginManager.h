#pragma once


#include <string>
#include <vector>
#include <map>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PluginLibrary;
class Plugin;


class GLOPERATE_API PluginManager {


    public:
        static void init(const std::string & executablePath = "");


    protected:
        /** Default path to look for plugins, by default the path of the executable */
        static std::string s_defaultPluginPath;


    public:
        PluginManager();
        virtual ~PluginManager();

        std::string scanDirectory() const;
        void setScanDirectory(const std::string & path);
        void scan(const std::string & identifier = "");
        void load(const std::string & name);
        void loadLibrary(const std::string & filename);
        const std::vector<Plugin *> & plugins() const;
        Plugin * plugin(const std::string & name) const;
        void printPlugins() const;


    protected:
        std::string                     m_scanDirectory;
        std::vector<PluginLibrary *>    m_libraries;
        std::vector<Plugin *>           m_plugins;
        std::map<std::string, Plugin *> m_pluginsByName;


};


} // namespace gloperate
