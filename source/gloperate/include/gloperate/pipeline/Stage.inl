
#pragma once


namespace gloperate
{


template <typename T>
Input<T> & Stage::createNewInput(const std::string & name, const T & defaultValue)
{
    auto newInput = new Input<T>(name, this, defaultValue);
    m_dynamicSlots.push_back(newInput);
    return (*newInput);
}

template <typename T>
Parameter<T> & Stage::createNewParameter(const std::string & name, const T & defaultValue)
{
    auto newParameter = new Parameter<T>(name, this, defaultValue);
    m_dynamicSlots.push_back(newParameter);
    return (*newParameter);
}

template <typename T>
Output<T> & Stage::createNewOutput(const std::string & name, const T & defaultValue)
{
    auto newOutput = new Output<T>(name, this, defaultValue);
    m_dynamicSlots.push_back(newOutput);
    return (*newOutput);
}

template <typename T>
ProxyOutput<T> & Stage::createNewProxyOutput(const std::string & name, const T & defaultValue)
{
    auto newProxyOutput = new ProxyOutput<T>(name, this, defaultValue);
    m_dynamicSlots.push_back(newProxyOutput);
    return (*newProxyOutput);
}


} // namespace gloperate
