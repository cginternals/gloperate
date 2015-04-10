
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
        Capability * castCapability = dynamic_cast<Capability *>(capability.get());

        if (castCapability != nullptr)
        {
            return castCapability;
        }
    }

    return nullptr;
}

template <typename Capability>
Capability * Painter::addCapability(std::unique_ptr<Capability> capability)
{
    auto abstractCapability = std::unique_ptr<AbstractCapability>{std::move(capability)};
    return static_cast<Capability *>(addCapability(std::move(abstractCapability)));
}


} // namespace gloperate
