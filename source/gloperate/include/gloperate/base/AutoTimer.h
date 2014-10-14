#pragma once


#include <string>
#include <memory>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class ChronoTimer;


class GLOPERATE_API AutoTimer
{
public:
    AutoTimer(const std::string & info);
    virtual ~AutoTimer();

protected:
    static int s_numActiveInstances;

    std::string m_info;
    int m_index;

    std::unique_ptr<ChronoTimer> m_timer;
};


} // namespace gloperate
