/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/QtEventTransformer.h>

#include <gloperate/input/input.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>

using namespace gloperate;

namespace gloperate_qt
{

/**
*  @brief
*    Constructor
*/
QtEventTransformer::QtEventTransformer()
{
}

/**
*  @brief
*    Destructor
*/
QtEventTransformer::~QtEventTransformer()
{
}

/**
*  @brief
*    Convert Qt event type into gloperate KeyboardEvent::Type
*/
KeyboardEvent::Type QtEventTransformer::keyboardTypeFromQtType(QEvent::Type type)
{
    // TODO make complete
    switch(type)
    {
    case QEvent::KeyPress:
        return KeyboardEvent::Type::Press;
    case QEvent::KeyRelease:
        return KeyboardEvent::Type::Release;
    default:
        return KeyboardEvent::Type::Press;
    }
}

/**
*  @brief
*    Convert Qt event type into gloperate MouseEvent::Type
*/
MouseEvent::Type QtEventTransformer::mouseTypeFromQtType(QEvent::Type type)
{
    // TODO make complete
    switch(type)
    {
    case QEvent::MouseButtonPress:
        return MouseEvent::Type::Press;
    case QEvent::MouseButtonRelease:
        return MouseEvent::Type::Release;
    case QEvent::MouseButtonDblClick:
        return MouseEvent::Type::DoubleClick;
    case QEvent::MouseMove:
        return MouseEvent::Type::Move;
    case QEvent::Enter:
        return MouseEvent::Type::Enter;
    case QEvent::Leave:
        return MouseEvent::Type::Leave;
    default:
        return MouseEvent::Type::Press;
    }
}

/**
*  @brief
*    Convert QPoint into glm::ivec2
*/
glm::ivec2 QtEventTransformer::fromQPoint(const QPoint point)
{
    return glm::ivec2(point.x(), point.y());
}

/**
*  @brief
*    Convert Qt mouse button into gloperate mouse button
*/
MouseButton QtEventTransformer::fromQtMouseButton(Qt::MouseButton button)
{
    if (button & Qt::LeftButton)
        return MouseButton::MouseButtonLeft;
    else if (button & Qt::RightButton)
        return MouseButton::MouseButtonRight;
    else if (button & Qt::MiddleButton)
        return MouseButton::MouseButtonMiddle;
    else if (button & Qt::XButton1)
        return MouseButton::MouseButton4;
    else if (button & Qt::XButton2)
        return MouseButton::MouseButton5;
    else
        return MouseButton::NoMouseButton;
}

MouseButton QtEventTransformer::fromQtMouseButtons(Qt::MouseButtons button)
{
    auto buttonMask = MouseButton::NoMouseButton;
    if (button & Qt::LeftButton)
        buttonMask |= MouseButton::MouseButtonLeft;
    if (button & Qt::RightButton)
        buttonMask |= MouseButton::MouseButtonRight;
    if (button & Qt::MiddleButton)
        buttonMask |= MouseButton::MouseButtonMiddle;
    if (button & Qt::XButton1)
        buttonMask |= MouseButton::MouseButton4;
    if (button & Qt::XButton2)
        buttonMask |= MouseButton::MouseButton5;

    return buttonMask;
}

/**
*  @brief
*    Convert Qt key code into gloperate key code
*/
Key QtEventTransformer::fromQtKeyCode(int key, int mods)
{
    if (mods & Qt::KeypadModifier) {
        switch (key)
        {
        case Qt::Key_0:
            return gloperate::Key::KeyKeypad0;
        case Qt::Key_1:
            return gloperate::Key::KeyKeypad1;
        case Qt::Key_2:
            return gloperate::Key::KeyKeypad2;
        case Qt::Key_3:
            return gloperate::Key::KeyKeypad3;
        case Qt::Key_4:
            return gloperate::Key::KeyKeypad4;
        case Qt::Key_5:
            return gloperate::Key::KeyKeypad5;
        case Qt::Key_6:
            return gloperate::Key::KeyKeypad6;
        case Qt::Key_7:
            return gloperate::Key::KeyKeypad7;
        case Qt::Key_8:
            return gloperate::Key::KeyKeypad8;
        case Qt::Key_9:
            return gloperate::Key::KeyKeypad9;
        case Qt::Key_division:
            return gloperate::Key::KeyKeypadDivide;
        case Qt::Key_multiply:
            return gloperate::Key::KeyKeypadMultiply;
        case Qt::Key_Minus:
            return gloperate::Key::KeyKeypadSubtract;
        case Qt::Key_Plus:
            return gloperate::Key::KeyKeypadAdd;
        case Qt::Key_Enter:
            return gloperate::Key::KeyKeypadEnter;
        case Qt::Key_Equal:
            return gloperate::Key::KeyKeypadEqual;
        default:
            return gloperate::Key::KeyUnknown;
        };
    } else {
        switch (key)
        {
        case Qt::Key_Space:
            return gloperate::Key::KeySpace;
        case Qt::Key_Apostrophe:
            return gloperate::Key::KeyApostrophe;
        case Qt::Key_Comma:
            return gloperate::Key::KeyComma;
        case Qt::Key_Minus:
            return gloperate::Key::KeyMinus;
        case Qt::Key_Period:
            return gloperate::Key::KeyPeriod;
        case Qt::Key_Slash:
            return gloperate::Key::KeySlash;
        case Qt::Key_0:
            return gloperate::Key::Key0;
        case Qt::Key_1:
            return gloperate::Key::Key1;
        case Qt::Key_2:
            return gloperate::Key::Key2;
        case Qt::Key_3:
            return gloperate::Key::Key3;
        case Qt::Key_4:
            return gloperate::Key::Key4;
        case Qt::Key_5:
            return gloperate::Key::Key5;
        case Qt::Key_6:
            return gloperate::Key::Key6;
        case Qt::Key_7:
            return gloperate::Key::Key7;
        case Qt::Key_8:
            return gloperate::Key::Key8;
        case Qt::Key_9:
            return gloperate::Key::Key9;
        case Qt::Key_Semicolon:
            return gloperate::Key::KeySemicolon;
        case Qt::Key_Equal:
            return gloperate::Key::KeyEqual;
        case Qt::Key_A:
            return gloperate::Key::KeyA;
        case Qt::Key_B:
            return gloperate::Key::KeyB;
        case Qt::Key_C:
            return gloperate::Key::KeyC;
        case Qt::Key_D:
            return gloperate::Key::KeyD;
        case Qt::Key_E:
            return gloperate::Key::KeyE;
        case Qt::Key_F:
            return gloperate::Key::KeyF;
        case Qt::Key_G:
            return gloperate::Key::KeyG;
        case Qt::Key_H:
            return gloperate::Key::KeyH;
        case Qt::Key_I:
            return gloperate::Key::KeyI;
        case Qt::Key_J:
            return gloperate::Key::KeyJ;
        case Qt::Key_K:
            return gloperate::Key::KeyK;
        case Qt::Key_L:
            return gloperate::Key::KeyL;
        case Qt::Key_M:
            return gloperate::Key::KeyM;
        case Qt::Key_N:
            return gloperate::Key::KeyN;
        case Qt::Key_O:
            return gloperate::Key::KeyO;
        case Qt::Key_P:
            return gloperate::Key::KeyP;
        case Qt::Key_Q:
            return gloperate::Key::KeyQ;
        case Qt::Key_R:
            return gloperate::Key::KeyR;
        case Qt::Key_S:
            return gloperate::Key::KeyS;
        case Qt::Key_T:
            return gloperate::Key::KeyT;
        case Qt::Key_U:
            return gloperate::Key::KeyU;
        case Qt::Key_V:
            return gloperate::Key::KeyV;
        case Qt::Key_W:
            return gloperate::Key::KeyW;
        case Qt::Key_X:
            return gloperate::Key::KeyX;
        case Qt::Key_Y:
            return gloperate::Key::KeyY;
        case Qt::Key_Z:
            return gloperate::Key::KeyZ;
        case Qt::Key_BracketLeft:
            return gloperate::Key::KeyLeftBracket;
        case Qt::Key_Backslash:
            return gloperate::Key::KeyBackslash;
        case Qt::Key_BracketRight:
            return gloperate::Key::KeyRightBracket;
        case Qt::Key_Agrave:
            return gloperate::Key::KeyGraveAccent;
        case Qt::Key_Escape:
            return gloperate::Key::KeyEscape;
        case Qt::Key_Return:
            return gloperate::Key::KeyEnter;
        case Qt::Key_Tab:
            return gloperate::Key::KeyTab;
        case Qt::Key_Backspace:
            return gloperate::Key::KeyBackspace;
        case Qt::Key_Insert:
            return gloperate::Key::KeyInsert;
        case Qt::Key_Delete:
            return gloperate::Key::KeyDelete;
        case Qt::Key_Right:
            return gloperate::Key::KeyRight;
        case Qt::Key_Left:
            return gloperate::Key::KeyLeft;
        case Qt::Key_Down:
            return gloperate::Key::KeyDown;
        case Qt::Key_Up:
            return gloperate::Key::KeyUp;
        case Qt::Key_PageUp:
            return gloperate::Key::KeyPageUp;
        case Qt::Key_PageDown:
            return gloperate::Key::KeyPageDown;
        case Qt::Key_Home:
            return gloperate::Key::KeyHome;
        case Qt::Key_End:
            return gloperate::Key::KeyEnd;
        case Qt::Key_CapsLock:
            return gloperate::Key::KeyCapsLock;
        case Qt::Key_ScrollLock:
            return gloperate::Key::KeyScrollLock;
        case Qt::Key_NumLock:
            return gloperate::Key::KeyNumLock;
        case Qt::Key_Print:
            return gloperate::Key::KeyPrintScreen;
        case Qt::Key_Pause:
            return gloperate::Key::KeyPause;
        case Qt::Key_F1:
            return gloperate::Key::KeyF1;
        case Qt::Key_F2:
            return gloperate::Key::KeyF2;
        case Qt::Key_F3:
            return gloperate::Key::KeyF3;
        case Qt::Key_F4:
            return gloperate::Key::KeyF4;
        case Qt::Key_F5:
            return gloperate::Key::KeyF5;
        case Qt::Key_F6:
            return gloperate::Key::KeyF6;
        case Qt::Key_F7:
            return gloperate::Key::KeyF7;
        case Qt::Key_F8:
            return gloperate::Key::KeyF8;
        case Qt::Key_F9:
            return gloperate::Key::KeyF9;
        case Qt::Key_F10:
            return gloperate::Key::KeyF10;
        case Qt::Key_F11:
            return gloperate::Key::KeyF11;
        case Qt::Key_F12:
            return gloperate::Key::KeyF12;
        case Qt::Key_F13:
            return gloperate::Key::KeyF13;
        case Qt::Key_F14:
            return gloperate::Key::KeyF14;
        case Qt::Key_F15:
            return gloperate::Key::KeyF15;
        case Qt::Key_F16:
            return gloperate::Key::KeyF16;
        case Qt::Key_F17:
            return gloperate::Key::KeyF17;
        case Qt::Key_F18:
            return gloperate::Key::KeyF18;
        case Qt::Key_F19:
            return gloperate::Key::KeyF19;
        case Qt::Key_F20:
            return gloperate::Key::KeyF20;
        case Qt::Key_F21:
            return gloperate::Key::KeyF21;
        case Qt::Key_F22:
            return gloperate::Key::KeyF22;
        case Qt::Key_F23:
            return gloperate::Key::KeyF23;
        case Qt::Key_F24:
            return gloperate::Key::KeyF24;
        case Qt::Key_F25:
            return gloperate::Key::KeyF25;
        case Qt::Key_Shift:
            return gloperate::Key::KeyLeftShift;
        case Qt::Key_Control:
            return gloperate::Key::KeyLeftControl;
        case Qt::Key_Alt:
            return gloperate::Key::KeyLeftAlt;
        case Qt::Key_Super_L:
            return gloperate::Key::KeyLeftSuper;
        case Qt::Key_AltGr:
            return gloperate::Key::KeyRightAlt;
        case Qt::Key_Super_R:
            return gloperate::Key::KeyRightSuper;
        case Qt::Key_Menu:
            return gloperate::Key::KeyMenu;
        default:
            return gloperate::Key::KeyUnknown;
        }
    }
}

KeyModifier QtEventTransformer::fromQtKeyboardModifiers(Qt::KeyboardModifiers modifiers)
{
    KeyModifier result = KeyModifier::ModNone;
    if (modifiers & Qt::ShiftModifier)
        result |= KeyModifier::ModShift;
    if (modifiers & Qt::ControlModifier)
        result |= KeyModifier::ModControl;
    if (modifiers & Qt::AltModifier)
        result |= KeyModifier::ModAlt;
    if (modifiers & Qt::MetaModifier)
        result |= KeyModifier::ModSuper;

    return result;
}

} // namespace gloperate_qt
