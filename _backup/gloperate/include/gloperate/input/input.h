
#pragma once


namespace gloperate
{


/**
*  @brief
*    Mouse buttons
*/
enum MouseButton {
    NoMouseButton = 0,
    MouseButton1 = 1,
    MouseButtonLeft = 1,
    MouseButton2 = 2,
    MouseButtonRight = 2,
    MouseButton3 = 3,
    MouseButtonMiddle = 3,
    MouseButton4,
    MouseButton5,
    MouseButton6,
    MouseButton7,
    MouseButton8
};


/**
*  @brief
*    Keyboard modifier keys
*/
enum KeyModifier {
    ModShift = 0x0001,
    ModControl = 0x0002,
    ModAlt = 0x0004,
    ModSuper = 0x0008
};


/**
*  @brief
*    Virtual key codes
*/
enum Key {
    KeyUnknown = -1,

    KeySpace = 32,
    KeyApostrophe = 39,
    KeyComma = 44,
    KeyMinus,
    KeyPeriod,
    KeySlash,

    Key0 = 48,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,

    KeySemicolon = 59,
    KeyEqual = 61,

    KeyA = 65,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,

    KeyLeftBracket = 91,
    KeyBackslash,
    KeyRightBracket,
    KeyGraveAccent = 96,
    KeyWorld1 = 161,
    KeyWorld2,
    KeyEscape = 256,
    KeyEnter,
    KeyTab,
    KeyBackspace,
    KeyInsert,
    KeyDelete,
    KeyRight,
    KeyLeft,
    KeyDown,
    KeyUp,
    KeyPageUp,
    KeyPageDown,
    KeyHome,
    KeyEnd,
    KeyCapsLock = 280,
    KeyScrollLock,
    KeyNumLock,
    KeyPrintScreen,
    KeyPause,

    KeyF1 = 290,
    KeyF2,
    KeyF3,
    KeyF4,
    KeyF5,
    KeyF6,
    KeyF7,
    KeyF8,
    KeyF9,
    KeyF10,
    KeyF11,
    KeyF12,
    KeyF13,
    KeyF14,
    KeyF15,
    KeyF16,
    KeyF17,
    KeyF18,
    KeyF19,
    KeyF20,
    KeyF21,
    KeyF22,
    KeyF23,
    KeyF24,
    KeyF25,

    KeyKeypad0 = 320,
    KeyKeypad1,
    KeyKeypad2,
    KeyKeypad3,
    KeyKeypad4,
    KeyKeypad5,
    KeyKeypad6,
    KeyKeypad7,
    KeyKeypad8,
    KeyKeypad9,
    KeyKeypadDecimal,
    KeyKeypadDivide,
    KeyKeypadMultiply,
    KeyKeypadSubtract,
    KeyKeypadAdd,
    KeyKeypadEnter,
    KeyKeypadEqual,

    KeyLeftShift = 340,
    KeyLeftControl,
    KeyLeftAlt,
    KeyLeftSuper,
    KeyRightShift,
    KeyRightControl,
    KeyRightAlt,
    KeyRightSuper,
    KeyMenu,

    KeyLast = KeyMenu
};


} // namespace gloperate
