
#pragma once


#include <gloperate/painter/AbstractCapability.h>


namespace gloperate
{


template <typename T>
bool AbstractCapability::is() const
{
    return dynamic_cast<T*>(this) != nullptr;
}

template <typename T>
T* AbstractCapability::as()
{
    return dynamic_cast<T*>(this);
}


}
