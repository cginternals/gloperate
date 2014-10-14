#pragma once


#include <vector>
#include <gloperate/capabilities/AbstractInputCapability.h>


namespace gloperate {


class MouseInputHandler;
class KeyboardInputHandler;


class GLOPERATE_API InputCapability : public AbstractInputCapability
{


public:
    InputCapability();
    virtual ~InputCapability();

    const std::vector<MouseInputHandler *> &mouseHandlers() const;
    void addMouseHandler(MouseInputHandler * handler);
    void removeMouseHandler(MouseInputHandler * handler);
    const std::vector<KeyboardInputHandler *> &keyboardHandlers() const;
    void addKeyboardHandler(KeyboardInputHandler * handler);
    void removeKeyboardHandler(KeyboardInputHandler * handler);

    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseWheel(int dx, int dy) override;
    virtual void onKeyDown(gloperate::Key key) override;
    virtual void onKeyUp(gloperate::Key key) override;


protected:
    std::vector<MouseInputHandler *>    m_mouseHandlers;
    std::vector<KeyboardInputHandler *> m_keyboardHandlers;


};


} // namespace gloperate
