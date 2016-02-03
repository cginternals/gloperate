
#pragma once


#include <gloperate/viewer/RenderHandler.h>


namespace gloperate
{


/**
*  @brief
*    Demo renderer handler that renders a simplen OpenGL scene
*/
class GLOPERATE_API DemoRenderer : public RenderHandler
{
public:
    /**
    *  @brief
    *    Constructor
    */
    DemoRenderer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoRenderer();

    // Virtual RenderHandler functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;
    virtual void onResize(int width, int height) override;
    virtual void onRender() override;
    virtual void onKeyPress(int key) override;
    virtual void onKeyRelease(int key) override;
    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int button, int x, int y) override;
    virtual void onMouseRelease(int button, int x, int y) override;
    virtual void onTimer(int id) override;
};


} // namespace gloperate
