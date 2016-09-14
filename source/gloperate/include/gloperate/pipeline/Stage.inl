
#pragma once


namespace gloperate
{


template <typename T>
Input<T> * Stage::createInput(const std::string & name, const T & defaultValue)
{
    auto input = new Input<T>(name, defaultValue);
    this->addInput(input, cppexpose::PropertyOwnership::Parent);

    return input;
}

template <typename T>
Parameter<T> * Stage::createParameter(const std::string & name, const T & defaultValue)
{
    auto parameter = new Parameter<T>(name, defaultValue);
    this->addParameter(parameter, cppexpose::PropertyOwnership::Parent);

    return parameter;
}

template <typename T>
Output<T> * Stage::createOutput(const std::string & name, const T & defaultValue)
{
    auto output = new Output<T>(name, defaultValue);
    this->addOutput(output, cppexpose::PropertyOwnership::Parent);

    return output;
}

template <typename T>
ProxyOutput<T> * Stage::createProxyOutput(const std::string & name, const T & defaultValue)
{
    auto proxy = new ProxyOutput<T>(name, defaultValue);
    this->addProxyOutput(proxy, cppexpose::PropertyOwnership::Parent);

    return proxy;
}


} // namespace gloperate
