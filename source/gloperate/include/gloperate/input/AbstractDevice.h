
#pragma once 

#include <gloperate/gloperate_api.h>

namespace gloperate
{
class AbstractDevice
{
public:
    /**
     * @brief
     *   Constructor
     */
    GLOPERATE_API AbstractDevice() = default;

    /**
     * @brief
     *   Destructor
     */
    GLOPERATE_API virtual ~AbstractDevice() = default ;

    /**
     * @brief
     *   Poll the current state of the Device and fire appropriate Signals
     */
    GLOPERATE_API virtual void update() = 0;

};

}

