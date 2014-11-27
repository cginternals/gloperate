#pragma once

namespace gloperate
{

/**
*  @brief
*    Check if a specific capability is supported by the painter
*/
template <typename Capability>
bool Painter::supports() const
{
    return getCapability<Capability>() != nullptr;
}

/**
*  @brief
*    Get a specific capability
*/
template <typename Capability>
Capability * Painter::getCapability() const
{
    for (AbstractCapability * capability : m_capabilities)
    {
        Capability * castCapability = dynamic_cast<Capability*>(capability);

        if (castCapability != nullptr)
        {
            return castCapability;
        }
    }
    return nullptr;
}

} // namespace gloperate
