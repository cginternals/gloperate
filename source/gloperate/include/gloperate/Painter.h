#pragma once


#include <vector>
#include <globjects/base/Referenced.h>
#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate
{


class ResourceManager;


class GLOPERATE_API Painter : public globjects::Referenced
{


public:
    Painter(ResourceManager & resourceManager);
    virtual ~Painter();

    void initialize();
    void paint();

    template <typename Capability>
    bool supports() const;

    template <typename Capability>
    Capability * getCapability() const;


protected:
    virtual void onInitialize() = 0;
    virtual void onPaint() = 0;


protected:
    void addCapability(AbstractCapability * capability);


protected:
    ResourceManager                  & m_resourceManager;
    std::vector<AbstractCapability*>   m_capabilities;


};


} // namespace gloperate


#include <gloperate/Painter.hpp>
