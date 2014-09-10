#pragma once


#include <globjects/base/ref_ptr.h>
#include <gloperate/Painter.h>
#include <gloperate-glfw/WindowEventHandlerBase.h>


namespace gloperate_glfw
{


class GLOPERATE_GLFW_API WindowEventHandler : public WindowEventHandlerBase
{


public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    gloperate::Painter * painter() const;
    void setPainter(gloperate::Painter * painter);

    virtual void initialize(gloperate_glfw::Window & window) override;
	virtual void idle(Window & window) override;


protected:
    virtual void framebufferResizeEvent(ResizeEvent & event) override;
    virtual void paintEvent(PaintEvent & event) override;
    virtual void keyPressEvent(KeyEvent & event) override;


protected:
    globjects::ref_ptr<gloperate::Painter> m_painter;


};


} // namespace gloperate_glfw
