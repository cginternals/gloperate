
#pragma once 

#include <gloperate/gloperate_api.h>

#include <vector>


namespace gloperate
{

class AbstractControl;

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




    std::vector<AbstractControl *> m_controls;
};

}

