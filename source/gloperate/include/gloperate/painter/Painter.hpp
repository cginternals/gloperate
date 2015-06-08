
#pragma once

#include <gloperate/painter/Painter.h>


namespace gloperate
{


template <typename Capability>
bool Painter::supports() const
{
    return getCapability<Capability>() != nullptr;
}

template <typename Capability>
Capability * Painter::getCapability() const
{
    for (auto & capability : m_capabilities)
    {
        Capability * castCapability = dynamic_cast<Capability *>(capability);

        if (castCapability != nullptr)
        {
            return castCapability;
        }
    }

    return nullptr;
}

template <typename Capability>
Capability * Painter::addCapability(Capability * capability)
{
    return static_cast<Capability *>(addCapability(static_cast<AbstractCapability *>(capability)));
}


} // namespace gloperate
