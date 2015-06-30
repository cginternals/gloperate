
#include <gloperate-osg/OsgKeyboardHandler.h>

#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>

#include <gloperate-osg/OsgRenderStage.h>


using namespace gloperate;

namespace gloperate_osg
{


/**
*  @brief
*    Convert gloperate key code into OSG key code
*/
static int toOsgKey(gloperate::Key key)
{
    switch (key) 
    {
    case KeySpace: return osgGA::GUIEventAdapter::KEY_Space;
    case KeyApostrophe: return osgGA::GUIEventAdapter::KEY_Quote;
    case KeyComma: return osgGA::GUIEventAdapter::KEY_Comma;
    case KeyMinus: return osgGA::GUIEventAdapter::KEY_Minus;
    case KeyPeriod: return osgGA::GUIEventAdapter::KEY_Period;
    case KeySlash: return osgGA::GUIEventAdapter::KEY_Slash;
    case Key0: return osgGA::GUIEventAdapter::KEY_0;
    case Key1: return osgGA::GUIEventAdapter::KEY_1;
    case Key2: return osgGA::GUIEventAdapter::KEY_2;
    case Key3: return osgGA::GUIEventAdapter::KEY_3;
    case Key4: return osgGA::GUIEventAdapter::KEY_4;
    case Key5: return osgGA::GUIEventAdapter::KEY_5;
    case Key6: return osgGA::GUIEventAdapter::KEY_6;
    case Key7: return osgGA::GUIEventAdapter::KEY_7;
    case Key8: return osgGA::GUIEventAdapter::KEY_8;
    case Key9: return osgGA::GUIEventAdapter::KEY_9;
    case KeySemicolon: return osgGA::GUIEventAdapter::KEY_Semicolon;
    case KeyEqual: return osgGA::GUIEventAdapter::KEY_Equals;
    case KeyA: return osgGA::GUIEventAdapter::KEY_A;
    case KeyB: return osgGA::GUIEventAdapter::KEY_B;
    case KeyC: return osgGA::GUIEventAdapter::KEY_C;
    case KeyD: return osgGA::GUIEventAdapter::KEY_D;
    case KeyE: return osgGA::GUIEventAdapter::KEY_E;
    case KeyF: return osgGA::GUIEventAdapter::KEY_F;
    case KeyG: return osgGA::GUIEventAdapter::KEY_G;
    case KeyH: return osgGA::GUIEventAdapter::KEY_H;
    case KeyI: return osgGA::GUIEventAdapter::KEY_I;
    case KeyJ: return osgGA::GUIEventAdapter::KEY_J;
    case KeyK: return osgGA::GUIEventAdapter::KEY_K;
    case KeyL: return osgGA::GUIEventAdapter::KEY_L;
    case KeyM: return osgGA::GUIEventAdapter::KEY_M;
    case KeyN: return osgGA::GUIEventAdapter::KEY_N;
    case KeyO: return osgGA::GUIEventAdapter::KEY_O;
    case KeyP: return osgGA::GUIEventAdapter::KEY_P;
    case KeyQ: return osgGA::GUIEventAdapter::KEY_Q;
    case KeyR: return osgGA::GUIEventAdapter::KEY_R;
    case KeyS: return osgGA::GUIEventAdapter::KEY_S;
    case KeyT: return osgGA::GUIEventAdapter::KEY_T;
    case KeyU: return osgGA::GUIEventAdapter::KEY_U;
    case KeyV: return osgGA::GUIEventAdapter::KEY_V;
    case KeyW: return osgGA::GUIEventAdapter::KEY_W;
    case KeyX: return osgGA::GUIEventAdapter::KEY_X;
    case KeyY: return osgGA::GUIEventAdapter::KEY_Y;
    case KeyZ: return osgGA::GUIEventAdapter::KEY_Z;
    case KeyLeftBracket: return osgGA::GUIEventAdapter::KEY_Leftbracket;
    case KeyBackslash: return osgGA::GUIEventAdapter::KEY_Backslash;
    case KeyRightBracket: return osgGA::GUIEventAdapter::KEY_Rightbracket;
    case KeyGraveAccent: return osgGA::GUIEventAdapter::KEY_Backquote;
    case KeyEscape: return osgGA::GUIEventAdapter::KEY_Escape;
    case KeyEnter: return osgGA::GUIEventAdapter::KEY_Return;
    case KeyTab: return osgGA::GUIEventAdapter::KEY_Tab;
    case KeyBackspace: return osgGA::GUIEventAdapter::KEY_BackSpace;
    case KeyInsert: return osgGA::GUIEventAdapter::KEY_Insert;
    case KeyDelete: return osgGA::GUIEventAdapter::KEY_Delete;
    case KeyRight: return osgGA::GUIEventAdapter::KEY_Right;
    case KeyLeft: return osgGA::GUIEventAdapter::KEY_Left;
    case KeyDown: return osgGA::GUIEventAdapter::KEY_Down;
    case KeyUp: return osgGA::GUIEventAdapter::KEY_Up;
    case KeyPageUp: return osgGA::GUIEventAdapter::KEY_Page_Up;
    case KeyPageDown: return osgGA::GUIEventAdapter::KEY_Page_Down;
    case KeyHome: return osgGA::GUIEventAdapter::KEY_Home;
    case KeyEnd: return osgGA::GUIEventAdapter::KEY_End;
    case KeyCapsLock: return osgGA::GUIEventAdapter::KEY_Caps_Lock;
    case KeyScrollLock: return osgGA::GUIEventAdapter::KEY_Scroll_Lock;
    case KeyNumLock: return osgGA::GUIEventAdapter::KEY_Num_Lock;
    case KeyPrintScreen: return osgGA::GUIEventAdapter::KEY_Print;
    case KeyPause: return osgGA::GUIEventAdapter::KEY_Pause;
    case KeyF1: return osgGA::GUIEventAdapter::KEY_F1;
    case KeyF2: return osgGA::GUIEventAdapter::KEY_F2;
    case KeyF3: return osgGA::GUIEventAdapter::KEY_F3;
    case KeyF4: return osgGA::GUIEventAdapter::KEY_F4;
    case KeyF5: return osgGA::GUIEventAdapter::KEY_F5;
    case KeyF6: return osgGA::GUIEventAdapter::KEY_F6;
    case KeyF7: return osgGA::GUIEventAdapter::KEY_F7;
    case KeyF8: return osgGA::GUIEventAdapter::KEY_F8;
    case KeyF9: return osgGA::GUIEventAdapter::KEY_F9;
    case KeyF10: return osgGA::GUIEventAdapter::KEY_F10;
    case KeyF11: return osgGA::GUIEventAdapter::KEY_F11;
    case KeyF12: return osgGA::GUIEventAdapter::KEY_F12;
    case KeyF13: return osgGA::GUIEventAdapter::KEY_F13;
    case KeyF14: return osgGA::GUIEventAdapter::KEY_F14;
    case KeyF15: return osgGA::GUIEventAdapter::KEY_F15;
    case KeyF16: return osgGA::GUIEventAdapter::KEY_F16;
    case KeyF17: return osgGA::GUIEventAdapter::KEY_F17;
    case KeyF18: return osgGA::GUIEventAdapter::KEY_F18;
    case KeyF19: return osgGA::GUIEventAdapter::KEY_F19;
    case KeyF20: return osgGA::GUIEventAdapter::KEY_F20;
    case KeyF21: return osgGA::GUIEventAdapter::KEY_F21;
    case KeyF22: return osgGA::GUIEventAdapter::KEY_F22;
    case KeyF23: return osgGA::GUIEventAdapter::KEY_F23;
    case KeyF24: return osgGA::GUIEventAdapter::KEY_F24;
    case KeyF25: return osgGA::GUIEventAdapter::KEY_F25;
    case KeyKeypad0: return osgGA::GUIEventAdapter::KEY_KP_0;
    case KeyKeypad1: return osgGA::GUIEventAdapter::KEY_KP_1;
    case KeyKeypad2: return osgGA::GUIEventAdapter::KEY_KP_2;
    case KeyKeypad3: return osgGA::GUIEventAdapter::KEY_KP_3;
    case KeyKeypad4: return osgGA::GUIEventAdapter::KEY_KP_4;
    case KeyKeypad5: return osgGA::GUIEventAdapter::KEY_KP_5;
    case KeyKeypad6: return osgGA::GUIEventAdapter::KEY_KP_6;
    case KeyKeypad7: return osgGA::GUIEventAdapter::KEY_KP_7;
    case KeyKeypad8: return osgGA::GUIEventAdapter::KEY_KP_8;
    case KeyKeypad9: return osgGA::GUIEventAdapter::KEY_KP_9;
    case KeyKeypadDecimal: return osgGA::GUIEventAdapter::KEY_KP_Decimal;
    case KeyKeypadDivide: return osgGA::GUIEventAdapter::KEY_KP_Divide;
    case KeyKeypadMultiply: return osgGA::GUIEventAdapter::KEY_KP_Multiply;
    case KeyKeypadSubtract: return osgGA::GUIEventAdapter::KEY_KP_Subtract;
    case KeyKeypadAdd: return osgGA::GUIEventAdapter::KEY_KP_Add;
    case KeyKeypadEnter: return osgGA::GUIEventAdapter::KEY_KP_Enter;
    case KeyKeypadEqual: return osgGA::GUIEventAdapter::KEY_KP_Equal;
    case KeyLeftShift: return osgGA::GUIEventAdapter::KEY_Shift_L;
    case KeyLeftControl: return osgGA::GUIEventAdapter::KEY_Control_L;
    case KeyLeftAlt: return osgGA::GUIEventAdapter::KEY_Alt_L;
    case KeyLeftSuper: return osgGA::GUIEventAdapter::KEY_Super_L;
    case KeyRightShift: return osgGA::GUIEventAdapter::KEY_Shift_R;
    case KeyRightControl: return osgGA::GUIEventAdapter::KEY_Control_R;
    case KeyRightAlt: return osgGA::GUIEventAdapter::KEY_Alt_R;
    case KeyRightSuper: return osgGA::GUIEventAdapter::KEY_Super_R;
    case KeyMenu: return osgGA::GUIEventAdapter::KEY_Menu;
    default: return -1;
    }
}


OsgKeyboardHandler::OsgKeyboardHandler(osgViewer::GraphicsWindowEmbedded * embedded, OsgRenderStage * stage)
: m_embedded(embedded)
, m_stage(stage)
{
    if (m_embedded) {
        m_embedded->ref();
    }
}

OsgKeyboardHandler::~OsgKeyboardHandler()
{
    if (m_embedded) {
        m_embedded->unref();
    }
}

void OsgKeyboardHandler::onKeyDown(gloperate::Key key)
{
    if (m_embedded) m_embedded->getEventQueue()->keyPress(toOsgKey(key));

    if (m_stage) m_stage->scheduleProcess();
}

void OsgKeyboardHandler::onKeyUp(gloperate::Key key)
{
    if (m_embedded) m_embedded->getEventQueue()->keyRelease(toOsgKey(key));

    if (m_stage) m_stage->scheduleProcess();
}


} // namespace gloperate_osg
