#pragma once

namespace gloperate
{

template <typename T>
gloperate::Data<T> * OutputCapability::getOutput(const std::string & name) const
{
    return m_pipeline.getOutput<double>(name);
}

template <typename T>
gloperate::Data<T> * OutputCapability::getOutput() const
{
    return m_pipeline.getOutput<double>();
}

} // namespace gloperate