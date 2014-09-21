#include <gloperate/capabilities/InputCapability.h>
#include <gloperate/input/MouseInputHandler.h>
#include <gloperate/input/KeyboardInputHandler.h>
#include <algorithm>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
InputCapability::InputCapability()
{
}

/**
*  @brief
*    Destructor
*/
InputCapability::~InputCapability()
{
    // Destroy mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        delete handler;
    }

    // Destroy keyboard handlers
    for (KeyboardInputHandler * handler : m_keyboardHandlers) {
        delete handler;
    }
}

/**
*  @brief
*    Get list of registered mouse handlers
*/
const std::vector<MouseInputHandler *> &InputCapability::mouseHandlers() const
{
    return m_mouseHandlers;
}

/**
*  @brief
*    Add mouse handler
*/
void InputCapability::addMouseHandler(MouseInputHandler * handler)
{
    m_mouseHandlers.push_back(handler);
}

/**
*  @brief
*    Remove mouse handler
*/
void InputCapability::removeMouseHandler(MouseInputHandler * handler)
{
    // Remove mouse handler from list. Do not destroy it, return ownership to the caller
    auto it = std::find(m_mouseHandlers.begin(), m_mouseHandlers.end(), handler);
    if (it != m_mouseHandlers.end()) {
        m_mouseHandlers.erase(it);
    }
}

/**
*  @brief
*    Get list of registered keyboard handlers
*/
const std::vector<KeyboardInputHandler *> &InputCapability::keyboardHandlers() const
{
    return m_keyboardHandlers;
}

/**
*  @brief
*    Add keyboard handler
*/
void InputCapability::addKeyboardHandler(KeyboardInputHandler * handler)
{
    m_keyboardHandlers.push_back(handler);
}

/**
*  @brief
*    Remove keyboard handler
*/
void InputCapability::removeKeyboardHandler(KeyboardInputHandler * handler)
{
    // Remove keyboard handler from list. Do not destroy it, return ownership to the caller
    auto it = std::find(m_keyboardHandlers.begin(), m_keyboardHandlers.end(), handler);
    if (it != m_keyboardHandlers.end()) {
        m_keyboardHandlers.erase(it);
    }
}

void InputCapability::onMouseMove(int x, int y)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseMove(x, y);
    }
}

void InputCapability::onMousePress(int x, int y, int button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMousePress(x, y, button);
    }
}

void InputCapability::onMouseRelease(int x, int y, int button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseRelease(x, y, button);
    }
}

void InputCapability::onMouseDoubleClick(int x, int y, int button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseDoubleClick(x, y, button);
    }
}

void InputCapability::onKeyDown(unsigned int key)
{
    // Call keyboard handlers
    for (KeyboardInputHandler * handler : m_keyboardHandlers) {
        handler->onKeyDown(key);
    }
}

void InputCapability::onKeyUp(unsigned int key)
{
    // Call keyboard handlers
    for (KeyboardInputHandler * handler : m_keyboardHandlers) {
        handler->onKeyUp(key);
    }
}


} // namespace gloperate
