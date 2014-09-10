#pragma once

#include <vector>

#include <globjects/base/Referenced.h>
#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate
{


class Viewport;


class GLOPERATE_API Painter : public globjects::Referenced
{


public:
    Painter();
    virtual ~Painter();

    void initialize();
    void resize(const Viewport & viewport);
    void paint();

    template <typename Capability>
    bool supports() const;

    template <typename Capability>
    Capability * getCapability() const;
protected:
    virtual void onInitialize()                      = 0;
    virtual void onResize(const Viewport & viewport) = 0;
    virtual void onPaint()                           = 0;

    void addCapability(AbstractCapability * capability);

protected:
    std::vector<AbstractCapability*> m_capabilities;
};

} // namespace gloperate

#include <gloperate/Painter.hpp>
