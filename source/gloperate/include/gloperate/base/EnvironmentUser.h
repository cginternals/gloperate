
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;


class GLOPERATE_API EnvironmentUser
{
public:
    virtual ~EnvironmentUser();


protected:
    explicit EnvironmentUser(Environment * environment);
    Environment * m_environment;
};


} // namespace gloperate
