
#pragma once


#include <glm/vec2.hpp>

#include <gloperate-headless/gloperate-headless_api.h>


namespace gloperate_headless
{


class Surface;


/**
*  @brief
*    Base class for surface events
*/
class GLOPERATE_HEADLESS_API SurfaceEvent
{
public:
    /**
    *  @brief
    *    Enumeration of all supported event types
    */
    enum class Type : unsigned int
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
      , Close
      , Paint
    };

    enum class ActionType : unsigned int
    {
        Release = 0,
        Press = 1,
        Repeat = 2
    };


public:
    /**
    *  @brief
    *    Destructor
    */
    virtual ~SurfaceEvent();

    /**
    *  @brief
    *    Get event type
    *
    *  @return
    *    The event type
    */
    Type type() const;

    /**
    *  @brief
    *    Get accepted state
    *
    *  @return
    *    The accepted state
    */
    bool isAccepted() const;

    /**
    *  @brief
    *    Get ignored state
    *
    *  @return
    *    The ignored state
    *
    *  @remarks
    *    An event is ignored iff it is not accepted
    */
    bool isIgnored() const;

    /**
    *  @brief
    *    Set accepted state
    *
    *  @param[in] accepted
    *    The new accepted state
    */
    void setAccepted(bool accepted);

    /**
    *  @brief
    *    Set accepted state to 'true'
    */
    void accept();

    /**
    *  @brief
    *    Set ignored state to 'true'
    *
    *  @remarks
    *    This is identical to setting the accepted state to 'false'
    */
    void ignore();

    /**
    *  @brief
    *    Get associated surface
    *
    *  @return
    *    The associated surface
    *
    *  @remarks
    *    May be 'null'
    */
    const Surface * surface() const;

    /**
    *  @brief
    *    Get associated surface
    *
    *  @return
    *    The associated surface
    *
    *  @remarks
    *    May be 'null'
    */
    Surface * surface();


    /**
    *  @brief
    *    Set associated surface
    *
    *  @param[in] surface
    *    The associated surface
    *
    *  @remarks
    *    Surface may be 'null'
    */
    void setSurface(Surface * surface);


protected:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    The event type
    *
    *  @remarks
    *    Initializes an event of given type that is not accepted
    *    and has no associated surface.
    */
    SurfaceEvent(Type type);


protected:
    Type     m_type;     ///< The event type
    bool     m_accepted; ///< The accepted state to prevent further bubbling
    Surface * m_surface; ///< The associated surface, may be null
};


/**
*  @brief
*    Representation of a keyboard event
*/
class GLOPERATE_HEADLESS_API KeyEvent : public SurfaceEvent
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
*    Representation of a mouse event (including scroll events)
*/
class GLOPERATE_HEADLESS_API MouseEvent : public SurfaceEvent
{
public:
    MouseEvent(const glm::ivec2 & pos);
    MouseEvent(const glm::ivec2 & pos, int button, int action, int modifiers);
    MouseEvent(const glm::ivec2 & pos, const glm::vec2 & delta, int modifiers);

    int button() const;
    int action() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;
    const glm::vec2 & delta() const;

    int x() const;
    int y() const;


protected:
    int m_button;
    int m_action;
    int m_modifiers;

    glm::ivec2 m_pos;
    glm::vec2  m_delta;
};


/**
*  @brief
*    Representation of a surface resize event (surface has been resized)
*/
class GLOPERATE_HEADLESS_API ResizeEvent : public SurfaceEvent
{
public:
    ResizeEvent(const glm::ivec2 & size);

    const glm::ivec2 & size() const;

    int width() const;
    int height() const;


protected:
    glm::ivec2 m_size;
};


/**
*  @brief
*    Representation of a paint event (surface has to be redrawn)
*/
class GLOPERATE_HEADLESS_API PaintEvent : public SurfaceEvent
{
public:
    PaintEvent();
};


/**
*  @brief
*    Representation of a surface close event (surface has been closed)
*/
class GLOPERATE_HEADLESS_API CloseEvent : public SurfaceEvent
{
public:
    CloseEvent();
};


} // namespace gloperate_headless
