/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <gloperate/gloperate_api.h>

#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {

class AbstractViewportCapability;

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API AbstractProjectionCapability : public AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractProjectionCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractProjectionCapability();

    virtual float aspectRatio() const = 0;

    virtual const glm::mat4 & projection() const = 0;

protected:
    virtual void setAspectRatio(float ratio) = 0;
    virtual void setAspectRatio(const glm::ivec2 & viewport) = 0;

    virtual void onViewportChanged();

protected:
    AbstractViewportCapability * m_viewportCapability;
};


} // namespace gloperate
