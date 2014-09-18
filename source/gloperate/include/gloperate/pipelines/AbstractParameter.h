#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/AbstractData.h>

namespace gloperate {
		
class GLOPERATE_API AbstractParameter : public virtual AbstractData
{
public:
	AbstractParameter();
	virtual ~AbstractParameter();

    const std::string & path() const;
    void setPath(const std::string & path);
    bool hasPath() const;

    virtual bool isParameter() const override;
    virtual bool isConvertible() const;
protected:
    std::string m_path;
};

} // namespace gloperate
