
#pragma once

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractControl
{
public:
    /**
     * @brief
     *   Constructor
     */
    GLOPERATE_API AbstractControl() = default;

    /**
     * @brief
     *   Destructor
     */
    GLOPERATE_API virtual ~AbstractControl() = default;
};

}
