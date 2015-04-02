#pragma once

#include <unordered_map>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/Painter.h>


namespace gloperate
{

class ResourceManager;
class AbstractPipeline;
class AbstractData;
template <typename T>
class Data;

class GLOPERATE_API PipelinePainter : public gloperate::Painter
{
public:
    PipelinePainter(gloperate::ResourceManager & resourceManager, AbstractPipeline & pipeline);
    virtual ~PipelinePainter();

    virtual void onInitialize() override;
    virtual void onPaint() override;

protected:
    AbstractPipeline & m_pipeline;
    std::unordered_map<gloperate::AbstractData *, reflectionzeug::AbstractProperty *> m_dataToPropertyMap;

    reflectionzeug::AbstractProperty * propertyFor(gloperate::AbstractData * parameter) const;
    reflectionzeug::AbstractProperty * property(const std::string & name) const;
    template <typename T>
    reflectionzeug::Property<T> * getProperty(const std::string & name) const;
    template <typename T>
    reflectionzeug::Property<T> * createProperty(const std::string & name, gloperate::Data<T> & data);
    template <typename T, typename K, typename V, typename... Args>
    reflectionzeug::Property<T> * createProperty(const std::string & name, gloperate::Data<T> & data, const K & key, const V & value, Args&&... args);
};

} // namespace gloperate

#include <gloperate/pipeline/PipelinePainter.hpp>
