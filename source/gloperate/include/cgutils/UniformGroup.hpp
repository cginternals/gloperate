#pragma once

#include <cgutils/UniformGroup.h>

namespace cgutils 
{

template <typename T>
glo::Uniform<T> * UniformGroup::uniform(const std::string & name)
{
    if (m_uniforms.count(name))
        return m_uniforms[name]->as<T>();

    // create new uniform if none named <name> exists

    glo::Uniform<T> * uniform = new glo::Uniform<T>(name);

    m_uniforms[uniform->name()] = uniform;

    return uniform;
}

template <typename T>
const glo::Uniform<T> * UniformGroup::uniform(const std::string & name) const
{
    if (m_uniforms.count(name))
        return m_uniforms.at(name)->as<T>();

    return nullptr;
}

} // namespace gloutils
