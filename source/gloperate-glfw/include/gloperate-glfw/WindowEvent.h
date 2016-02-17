
#pragma once


#include <chrono>

#include <glm/glm.hpp>

#include <gloperate-glfw/gloperate-glfw_api.h>


namespace gloperate_glfw
{


class Window;


/**
*  @brief
*    Base class for window events
*/
class GLOPERATE_GLFW_API WindowEvent
{
public:
    enum class Type
    {
        KeyPress
      , KeyRelease
      , KeyTyped
      , MousePress
      , MouseRelease
      , MouseMove
      , MouseEnter
      , MouseLeave
      , Scroll
      , Resize
      , FrameBufferResize
      , Move
      , Close
      , Focus
      , Iconify
      , Paint
    };


public:
    virtual ~WindowEvent();

    Type type() const;

    bool isAccepted() const;
    bool isIgnored() const;

    void setAccepted(bool accepted);
    void accept();
    void ignore();

    Window * window() const;
    void setWindow(Window * window);


protected:
    WindowEvent(Type type);


protected:
    Type m_type;
    bool m_accepted;

    Window * m_window;
};


/**
*  @brief
*    Representation of a keyboard event
*/
class GLOPERATE_GLFW_API KeyEvent : public WindowEvent
{
public:
    KeyEvent(int key, int scanCode, int action, int modifiers);
    explicit KeyEvent(unsigned int character);

    int key() const;
    int scanCode() const;
    int action() const;
    int modifiers() const;
    unsigned int character() const;


protected:
    int m_key;
    int m_scanCode;
    int m_action;
    int m_modifiers;
    unsigned int m_character;
};


/**
*  @brief
*    Representation of a mouse event
*/
class GLOPERATE_GLFW_API MouseEvent : public WindowEvent
{
public:
    MouseEvent(const glm::ivec2 & pos);
    MouseEvent(const glm::ivec2 & pos, int button, int action, int modifiers);

    int button() const;
    int action() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;


protected:
    int m_button;
    int m_action;
    int m_modifiers;

    glm::ivec2 m_pos;
};


/**
*  @brief
*    Representation of a mouse event (cursor enters the window)
*/
class GLOPERATE_GLFW_API MouseEnterEvent : public WindowEvent
{
public:
    MouseEnterEvent();
};


/**
*  @brief
*    Representation of a mouse event (cursor leaves the window)
*/
class GLOPERATE_GLFW_API MouseLeaveEvent : public WindowEvent
{
public:
    MouseLeaveEvent();
};


/**
*  @brief
*    Representation of a mouse scroll event
*/
class GLOPERATE_GLFW_API ScrollEvent : public WindowEvent
{
public:
    ScrollEvent(const glm::vec2 & offset, const glm::ivec2 & pos);

    const glm::vec2 & offset() const;
    const glm::ivec2 & pos() const;


protected:
    glm::vec2 m_offset;
    glm::ivec2 m_pos;
};


/**
*  @brief
*    Representation of a window move event (window has been moved)
*/
class GLOPERATE_GLFW_API MoveEvent : public WindowEvent
{
public:
    MoveEvent(const glm::ivec2 & pos);

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;


protected:
    glm::ivec2 m_pos;
};


/**
*  @brief
*    Representation of a window resize event (window has been resized)
*/
class GLOPERATE_GLFW_API ResizeEvent : public WindowEvent
{
public:
    ResizeEvent(const glm::ivec2 & size, bool framebuffer = false);

    const glm::ivec2 & size() const;

    int width() const;
    int height() const;


protected:
    glm::ivec2 m_size;
};


/**
*  @brief
*    Representation of a paint event (window has to be redrawn)
*/
class GLOPERATE_GLFW_API PaintEvent : public WindowEvent
{
public:
    PaintEvent();
};


/**
*  @brief
*    Representation of a window close event (window has been closed)
*/
class GLOPERATE_GLFW_API CloseEvent : public WindowEvent
{
public:
    CloseEvent();
};


/**
*  @brief
*    Representation of a focus event (window has received or lost the keyboard focus)
*/
class GLOPERATE_GLFW_API FocusEvent : public WindowEvent
{
public:
    FocusEvent(bool hasFocus);

    bool hasFocus() const;


protected:
    bool m_hasFocus;
};


/**
*  @brief
*    Representation of a window iconify event (window has been minimized)
*/
class GLOPERATE_GLFW_API IconifyEvent : public WindowEvent
{
public:
    IconifyEvent(bool isIconified);

    bool isIconified() const;


protected:
    bool m_isIconified;
};


} // namespace gloperate_glfw
