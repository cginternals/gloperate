#pragma once


#include <globjects-base/Referenced.h>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Viewport;


class GLOPERATE_API Painter : public glo::Referenced
{


public:
    Painter();
    virtual ~Painter();

    void initialize();
    void resize(const Viewport &viewport);
    void draw();


protected:
    virtual void onInitialize()                     = 0;
    virtual void onResize(const Viewport &viewport) = 0;
    virtual void onDraw()                           = 0;


protected:


};


} // namespace gloperate
