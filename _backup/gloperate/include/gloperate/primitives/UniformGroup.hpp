
#pragma once


#include <gloperate/primitives/UniformGroup.h>


namespace gloperate
{


template <typename T>
globjects::Uniform<T> * UniformGroup::uniform(const std::string & name)
{
    if (m_uniforms.count(name))
        return m_uniforms[name]->as<T>();

    // create new uniform if none named <name> exists

    globjects::Uniform<T> * uniform = new globjects::Uniform<T>(name);

    m_uniforms[uniform->name()] = uniform;

    return uniform;
}

template <typename T>
const globjects::Uniform<T> * UniformGroup::uniform(const std::string & name) const
{
    if (m_uniforms.count(name))
        return m_uniforms.at(name)->as<T>();

    return nullptr;
}


} // namespace gloperate
