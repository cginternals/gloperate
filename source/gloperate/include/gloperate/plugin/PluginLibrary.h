#pragma once


#include <string>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Plugin;


typedef void (*INITPLUGINLIB_PTR)();
typedef int (*GETNUMOFPLUGINS_PTR)();
typedef gloperate::Plugin * (*GETPLUGIN_PTR)(unsigned int);
typedef void (*DEINITPLUGINLIB_PTR)();


class GLOPERATE_API PluginLibrary {


    public:
        PluginLibrary(const std::string & filename);
        virtual ~PluginLibrary();

        const std::string & filename() const;
        bool isValid() const;
        void initialize();
        unsigned int getNumOfPlugins() const;
        gloperate::Plugin * getPlugin(unsigned int index) const;
        void deinitialize();


    protected:
        std::string         m_filename;
        INITPLUGINLIB_PTR   m_initPluginPtr;
        GETNUMOFPLUGINS_PTR m_getNumOfPluginsPtr;
        GETPLUGIN_PTR       m_getPluginPtr;
        DEINITPLUGINLIB_PTR m_deinitPluginPtr;


};


} // namespace gloperate
