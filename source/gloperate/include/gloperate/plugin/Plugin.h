#pragma once

#include <memory>
#include <string>

#include <gloperate/gloperate_api.h>

namespace gloperate_qt
{
	class QtOpenGLWindow;
}

namespace gloperate
{

class Painter;
class ResourceManager;


/** /brief Describes a plugin that can be loaded dynamically
*/
class GLOPERATE_API Plugin 
{
public:
    Plugin(
        const std::string & type
    ,   const std::string & name
    ,   const std::string & description
    ,   const std::string & vendor
    ,   const std::string & version);

    virtual ~Plugin();

    const char * type() const;
    const char * name() const;
    const char * description() const;
    const char * vendor() const;
    const char * version() const;

	virtual Painter * createPainter(ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas) const = 0;

protected:
    std::string m_type;
    std::string m_name;
    std::string m_description;
    std::string m_vendor;
    std::string m_version;
};

} // namespace gloperate
