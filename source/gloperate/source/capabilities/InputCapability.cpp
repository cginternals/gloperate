#include <gloperate/capabilities/InputCapability.h>
#include <gloperate/input/MouseInputHandler.h>
#include <gloperate/input/KeyboardInputHandler.h>
#include <algorithm>


namespace gloperate
{


InputCapability::InputCapability()
{
}

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

const std::vector<MouseInputHandler *> &InputCapability::mouseHandlers() const
{
    return m_mouseHandlers;
}

void InputCapability::addMouseHandler(MouseInputHandler * handler)
{
    m_mouseHandlers.push_back(handler);
}

void InputCapability::removeMouseHandler(MouseInputHandler * handler)
{
    // Remove mouse handler from list. Do not destroy it, return ownership to the caller
    auto it = std::find(m_mouseHandlers.begin(), m_mouseHandlers.end(), handler);
    if (it != m_mouseHandlers.end()) {
        m_mouseHandlers.erase(it);
    }
}

const std::vector<KeyboardInputHandler *> &InputCapability::keyboardHandlers() const
{
    return m_keyboardHandlers;
}

void InputCapability::addKeyboardHandler(KeyboardInputHandler * handler)
{
    m_keyboardHandlers.push_back(handler);
}

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

void InputCapability::onMousePress(int x, int y, gloperate::MouseButton button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMousePress(x, y, button);
    }
}

void InputCapability::onMouseRelease(int x, int y, gloperate::MouseButton button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseRelease(x, y, button);
    }
}

void InputCapability::onMouseDoubleClick(int x, int y, gloperate::MouseButton button)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseDoubleClick(x, y, button);
    }
}

void InputCapability::onMouseWheel(int dx, int dy)
{
    // Call mouse handlers
    for (MouseInputHandler * handler : m_mouseHandlers) {
        handler->onMouseWheel(dx, dy);
    }
}

void InputCapability::onKeyDown(gloperate::Key key)
{
    // Call keyboard handlers
    for (KeyboardInputHandler * handler : m_keyboardHandlers) {
        handler->onKeyDown(key);
    }
}

void InputCapability::onKeyUp(gloperate::Key key)
{
    // Call keyboard handlers
    for (KeyboardInputHandler * handler : m_keyboardHandlers) {
        handler->onKeyUp(key);
    }
}


} // namespace gloperate
