#pragma once


#include <globjects/base/Referenced.h>
#include <gloperate/gloperate_api.h>


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


protected:
    virtual void onInitialize()                      = 0;
    virtual void onResize(const Viewport & viewport) = 0;
    virtual void onPaint()                           = 0;


protected:


};


} // namespace gloperate
