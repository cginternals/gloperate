
#pragma once


#include <gloperate/viewer/Surface.h>


namespace gloperate
{


/**
*  @brief
*    Demo renderer handler that renders a simplen OpenGL scene
*/
class GLOPERATE_API DemoRenderer : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    DemoRenderer(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoRenderer();

    // Virtual Surface functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;
    virtual void onResize(int deviceWidth, int deviceHeight, int virtualWidth, int virtualHeight) override;
    virtual void onRender() override;
    virtual void onKeyPress(int key) override;
    virtual void onKeyRelease(int key) override;
    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int button, int x, int y) override;
    virtual void onMouseRelease(int button, int x, int y) override;
    virtual void onMouseWheel(float dx, float dy, int x, int y) override;
    virtual void onTimer(int id) override;
};


} // namespace gloperate
