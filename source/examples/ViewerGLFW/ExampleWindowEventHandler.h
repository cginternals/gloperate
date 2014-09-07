#pragma once


#include <string>
#include <gloperate-glfw/WindowEventHandler.h>
#include <gloperate-glfw/Timer.h>
#include <gloperate-glfw/events.h>


class ExampleWindowEventHandler : public gloperate_glfw::WindowEventHandler
{


public:
    ExampleWindowEventHandler();
    virtual ~ExampleWindowEventHandler();

    virtual void initialize(gloperate_glfw::Window & window) override;

    virtual void handleEvent(gloperate_glfw::WindowEvent & event) override;


protected:
    void setViewport(gloperate_glfw::ResizeEvent & event);
    void computeFps(gloperate_glfw::PaintEvent & event);
    void handleDefaultKeys(gloperate_glfw::KeyEvent & event);

protected:
    gloperate_glfw::Timer m_timer;
    long double           m_swapElapsedTime;
    unsigned int          m_swapCount;
    std::string           m_baseTitle;


};
