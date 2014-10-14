#pragma once


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
