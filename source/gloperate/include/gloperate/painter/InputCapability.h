#pragma once

#include <vector>
#include <gloperate/painter/AbstractInputCapability.h>


namespace gloperate 
{

class MouseInputHandler;
class KeyboardInputHandler;

/**
*  @brief
*    Default implementation for AbstractInputCapability
*/
class GLOPERATE_API InputCapability : public AbstractInputCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    InputCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~InputCapability();

    /**
    *  @brief
    *    Get list of registered mouse handlers
    *
    *  @return
    *    List of mouse handlers
    */
    const std::vector<MouseInputHandler *> &mouseHandlers() const;

    /**
    *  @brief
    *    Add mouse handler
    *
    *  @param[in] handler
    *    Mouse handler
    *
    *  @remarks
    *    The input capability takes ownership over the mouse handler
    *    and will delete it when it is appropriate.
    */
    void addMouseHandler(MouseInputHandler * handler);

    /**
    *  @brief
    *    Remove mouse handler
    *
    *  @param[in] handler
    *    Mouse handler
    *
    *  @remarks
    *    Transfers ownership over the mouse handler back to the caller.
    */
    void removeMouseHandler(MouseInputHandler * handler);

    /**
    *  @brief
    *    Get list of registered keyboard handlers
    *
    *  @return
    *    List of keyboard handlers
    */
    const std::vector<KeyboardInputHandler *> &keyboardHandlers() const;

    /**
    *  @brief
    *    Add keyboard handler
    *
    *  @param[in] handler
    *    Keyboard handler
    *
    *  @remarks
    *    The input capability takes ownership over the keyboard handler
    *    and will delete it when it is appropriate.
    */
    void addKeyboardHandler(KeyboardInputHandler * handler);

    /**
    *  @brief
    *    Remove keyboard handler
    *
    *  @param[in] handler
    *    Keyboard handler
    *
    *  @remarks
    *    Transfers ownership over the keyboard handler back to the caller.
    */
    void removeKeyboardHandler(KeyboardInputHandler * handler);

    // Virtual functions from AbstractInputCapability
    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseWheel(int dx, int dy) override;
    virtual void onKeyDown(gloperate::Key key) override;
    virtual void onKeyUp(gloperate::Key key) override;

protected:
    std::vector<MouseInputHandler *>    m_mouseHandlers;      /**< List of mouse handlers */
    std::vector<KeyboardInputHandler *> m_keyboardHandlers;   /**< List of keyboard handlers */
};

} // namespace gloperate
