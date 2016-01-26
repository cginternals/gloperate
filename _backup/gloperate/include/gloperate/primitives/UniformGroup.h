
#pragma once


#include <unordered_map>
#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/AbstractUniform.h>
#include <globjects/Uniform.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class AbstractUniform;
    class Program;
}


namespace gloperate
{


/**
*  @brief
*    Uniform group helper
*
*    A uniform group defines a number of uniforms and their
*    values and can be used to synchronize common uniforms
*    between different programs.
*/
class GLOPERATE_API UniformGroup
{
public:
    UniformGroup();
    virtual ~UniformGroup();

    template <typename T>
    globjects::Uniform<T> * uniform(const std::string & name);

    template <typename T>
    const globjects::Uniform<T> * uniform(const std::string & name) const;

    void addUniform(globjects::AbstractUniform * uniform);
    void addToProgram(globjects::Program * program);


protected:
    std::unordered_map<std::string, globjects::ref_ptr<globjects::AbstractUniform>> m_uniforms;
};


} // namespace gloperate


#include <gloperate/primitives/UniformGroup.hpp>
