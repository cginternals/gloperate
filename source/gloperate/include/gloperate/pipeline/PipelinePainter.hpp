#pragma once

#include <gloperate/pipeline/PipelinePainter.h>

namespace gloperate
{

template <typename T>
reflectionzeug::Property<T> * PipelinePainter::getProperty(const std::string & name) const
{
    return dynamic_cast<reflectionzeug::Property<T>*>(property(name));
}

template <typename T>
reflectionzeug::Property<T> * PipelinePainter::createProperty(const std::string & name, gloperate::Data<T> & data)
{
    auto property = new reflectionzeug::Property<T>(name,
       [& data]() { return data.data(); },
       [& data](const T & value) { data.setData(value); }
    );

    m_dataToPropertyMap[std::addressof(data)] = property;

    return property;
}

template <typename T, typename K, typename V, typename... Args>
reflectionzeug::Property<T> * PipelinePainter::createProperty(const std::string & name, gloperate::Data<T> & data, const K & key, const V & value, Args&&... args)
{
    auto property = createProperty(name, data, std::forward<Args>(args)...);

    property->setOption(key, value);

    return property;
}

} // namespace gloperate
