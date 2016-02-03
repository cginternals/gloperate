
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
    virtual void onResize(int width, int height) override;
    virtual void onRender() override;
    virtual void onKeyPressed(int key) override;
    virtual void onKeyReleased(int key) override;
    virtual void onMouseMoved(int x, int y) override;
    virtual void onMousePressed(int button, int x, int y) override;
    virtual void onMouseReleased(int button, int x, int y) override;
    virtual void onTimer(int id) override;
};


} // namespace gloperate
