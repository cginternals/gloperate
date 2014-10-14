/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/signals/Signal.h>


namespace gloperate {


class GLOPERATE_API AbstractCapability
{


public:
    AbstractCapability();
    virtual ~AbstractCapability();

    bool hasChanged() const;
    void setChanged(bool changed);


    Signal<> changed;
protected:
    bool m_changed; /**< Has the capability information been changed? */

};


} // namespace gloperate
