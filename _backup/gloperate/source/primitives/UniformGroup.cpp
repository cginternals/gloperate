
#include <gloperate/primitives/UniformGroup.h>

#include <cassert>

#include <globjects/logging.h>
#include <globjects/AbstractUniform.h>
#include <globjects/Program.h>


using namespace globjects;


namespace gloperate
{


UniformGroup::UniformGroup()
{
}

UniformGroup::~UniformGroup()
{
}

void UniformGroup::addUniform(AbstractUniform * uniform)
{
    assert(uniform != nullptr);

    const std::string name = uniform->name();

    if (m_uniforms.count(name) && m_uniforms.at(name).get() != uniform)
        globjects::warning() << "Uniform with name " << name << " already exists on UniformGroup, overwrite it.";

    m_uniforms[name] = uniform;
}

void UniformGroup::addToProgram(Program * program)
{
    assert(program != nullptr);

    for (std::pair<std::string, globjects::ref_ptr<AbstractUniform>> pair : m_uniforms)
        program->addUniform(pair.second);
}


} // namespace gloperate
