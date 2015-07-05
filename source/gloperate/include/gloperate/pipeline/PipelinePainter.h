
#pragma once


#include <unordered_map>

#include <gloperate/painter/Painter.h>


namespace gloperate
{


class ResourceManager;
class AbstractPipeline;
class AbstractData;

template <typename T>
class Data;


/**
*  @brief
*    Painter that executed a rendering pipeline
*
*  @see AbstractPipeline
*/
class GLOPERATE_API PipelinePainter : public gloperate::Painter
{
public:
    PipelinePainter(
        const std::string & name,
        gloperate::ResourceManager & resourceManager,
        const std::string & relDataPath,
        AbstractPipeline & pipeline);

    virtual void onInitialize() override;
    virtual void onPaint() override;


protected:
    reflectionzeug::AbstractProperty * propertyFor(gloperate::AbstractData * parameter) const;
    reflectionzeug::AbstractProperty * property(const std::string & name) const;
    template <typename T>
    reflectionzeug::Property<T> * getProperty(const std::string & name) const;
    template <typename T>
    reflectionzeug::Property<T> * createProperty(const std::string & name, gloperate::Data<T> & data);

    
protected:
    AbstractPipeline & m_pipeline;
    std::unordered_map<gloperate::AbstractData *, reflectionzeug::AbstractProperty *> m_dataToPropertyMap;
};


} // namespace gloperate


#include <gloperate/pipeline/PipelinePainter.hpp>
