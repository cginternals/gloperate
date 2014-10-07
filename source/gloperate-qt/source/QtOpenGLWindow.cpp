/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include "gloperate-qt/QtOpenGLWindow.h"
#include <gloperate-qt/qt-includes-begin.h>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <gloperate-qt/qt-includes-end.h>
#include <globjects/globjects.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/capabilities/AbstractInputCapability.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/tools/ScreenshotTool.h>


using namespace gloperate;
namespace gloperate_qt
{


/**
*  @brief
*    Convert Qt mouse button into gloperate mouse button
*/
static gloperate::MouseButton fromQtMouseButton(Qt::MouseButton button)
{
    if (button & Qt::LeftButton)
        return MouseButtonLeft;
    else if (button & Qt::RightButton)
        return MouseButtonRight;
    else if (button & Qt::MiddleButton)
        return MouseButtonMiddle;
    else if (button & Qt::XButton1)
        return MouseButton4;
    else if (button & Qt::XButton2)
        return MouseButton5;
    else
        return NoMouseButton;
}

/**
*  @brief
*    Convert Qt key code into gloperate key code
*/
static gloperate::Key fromQtKeyCode(int key, int mods)
{
    if (mods & Qt::KeypadModifier) {
        switch (key) {
            case Qt::Key_0: return gloperate::KeyKeypad0;
            case Qt::Key_1: return gloperate::KeyKeypad1;
            case Qt::Key_2: return gloperate::KeyKeypad2;
            case Qt::Key_3: return gloperate::KeyKeypad3;
            case Qt::Key_4: return gloperate::KeyKeypad4;
            case Qt::Key_5: return gloperate::KeyKeypad5;
            case Qt::Key_6: return gloperate::KeyKeypad6;
            case Qt::Key_7: return gloperate::KeyKeypad7;
            case Qt::Key_8: return gloperate::KeyKeypad8;
            case Qt::Key_9: return gloperate::KeyKeypad9;
            case Qt::Key_division: return gloperate::KeyKeypadDivide;
            case Qt::Key_multiply: return gloperate::KeyKeypadMultiply;
            case Qt::Key_Minus: return gloperate::KeyKeypadSubtract;
            case Qt::Key_Plus: return gloperate::KeyKeypadAdd;
            case Qt::Key_Enter: return gloperate::KeyKeypadEnter;
            case Qt::Key_Equal: return gloperate::KeyKeypadEqual;
            default: return gloperate::KeyUnknown;
        };
    } else {
        switch (key) {
            case Qt::Key_Space: return gloperate::KeySpace;
            case Qt::Key_Apostrophe: return gloperate::KeyApostrophe;
            case Qt::Key_Comma: return gloperate::KeyComma;
            case Qt::Key_Minus: return gloperate::KeyMinus;
            case Qt::Key_Period: return gloperate::KeyPeriod;
            case Qt::Key_Slash: return gloperate::KeySlash;
            case Qt::Key_0: return gloperate::Key0;
            case Qt::Key_1: return gloperate::Key1;
            case Qt::Key_2: return gloperate::Key2;
            case Qt::Key_3: return gloperate::Key3;
            case Qt::Key_4: return gloperate::Key4;
            case Qt::Key_5: return gloperate::Key5;
            case Qt::Key_6: return gloperate::Key6;
            case Qt::Key_7: return gloperate::Key7;
            case Qt::Key_8: return gloperate::Key8;
            case Qt::Key_9: return gloperate::Key9;
            case Qt::Key_Semicolon: return gloperate::KeySemicolon;
            case Qt::Key_Equal: return gloperate::KeyEqual;
            case Qt::Key_A: return gloperate::KeyA;
            case Qt::Key_B: return gloperate::KeyB;
            case Qt::Key_C: return gloperate::KeyC;
            case Qt::Key_D: return gloperate::KeyD;
            case Qt::Key_E: return gloperate::KeyE;
            case Qt::Key_F: return gloperate::KeyF;
            case Qt::Key_G: return gloperate::KeyG;
            case Qt::Key_H: return gloperate::KeyH;
            case Qt::Key_I: return gloperate::KeyI;
            case Qt::Key_J: return gloperate::KeyJ;
            case Qt::Key_K: return gloperate::KeyK;
            case Qt::Key_L: return gloperate::KeyL;
            case Qt::Key_M: return gloperate::KeyM;
            case Qt::Key_N: return gloperate::KeyN;
            case Qt::Key_O: return gloperate::KeyO;
            case Qt::Key_P: return gloperate::KeyP;
            case Qt::Key_Q: return gloperate::KeyQ;
            case Qt::Key_R: return gloperate::KeyR;
            case Qt::Key_S: return gloperate::KeyS;
            case Qt::Key_T: return gloperate::KeyT;
            case Qt::Key_U: return gloperate::KeyU;
            case Qt::Key_V: return gloperate::KeyV;
            case Qt::Key_W: return gloperate::KeyW;
            case Qt::Key_X: return gloperate::KeyX;
            case Qt::Key_Y: return gloperate::KeyY;
            case Qt::Key_Z: return gloperate::KeyZ;
            case Qt::Key_BracketLeft: return gloperate::KeyLeftBracket;
            case Qt::Key_Backslash: return gloperate::KeyBackslash;
            case Qt::Key_BracketRight: return gloperate::KeyRightBracket;
            case Qt::Key_Agrave: return gloperate::KeyGraveAccent;
            case Qt::Key_Escape: return gloperate::KeyEscape;
            case Qt::Key_Return: return gloperate::KeyEnter;
            case Qt::Key_Tab: return gloperate::KeyTab;
            case Qt::Key_Backspace: return gloperate::KeyBackspace;
            case Qt::Key_Insert: return gloperate::KeyInsert;
            case Qt::Key_Delete: return gloperate::KeyDelete;
            case Qt::Key_Right: return gloperate::KeyRight;
            case Qt::Key_Left: return gloperate::KeyLeft;
            case Qt::Key_Down: return gloperate::KeyDown;
            case Qt::Key_Up: return gloperate::KeyUp;
            case Qt::Key_PageUp: return gloperate::KeyPageUp;
            case Qt::Key_PageDown: return gloperate::KeyPageDown;
            case Qt::Key_Home: return gloperate::KeyHome;
            case Qt::Key_End: return gloperate::KeyEnd;
            case Qt::Key_CapsLock: return gloperate::KeyCapsLock;
            case Qt::Key_ScrollLock: return gloperate::KeyScrollLock;
            case Qt::Key_NumLock: return gloperate::KeyNumLock;
            case Qt::Key_Print: return gloperate::KeyPrintScreen;
            case Qt::Key_Pause: return gloperate::KeyPause;
            case Qt::Key_F1: return gloperate::KeyF1;
            case Qt::Key_F2: return gloperate::KeyF2;
            case Qt::Key_F3: return gloperate::KeyF3;
            case Qt::Key_F4: return gloperate::KeyF4;
            case Qt::Key_F5: return gloperate::KeyF5;
            case Qt::Key_F6: return gloperate::KeyF6;
            case Qt::Key_F7: return gloperate::KeyF7;
            case Qt::Key_F8: return gloperate::KeyF8;
            case Qt::Key_F9: return gloperate::KeyF9;
            case Qt::Key_F10: return gloperate::KeyF10;
            case Qt::Key_F11: return gloperate::KeyF11;
            case Qt::Key_F12: return gloperate::KeyF12;
            case Qt::Key_F13: return gloperate::KeyF13;
            case Qt::Key_F14: return gloperate::KeyF14;
            case Qt::Key_F15: return gloperate::KeyF15;
            case Qt::Key_F16: return gloperate::KeyF16;
            case Qt::Key_F17: return gloperate::KeyF17;
            case Qt::Key_F18: return gloperate::KeyF18;
            case Qt::Key_F19: return gloperate::KeyF19;
            case Qt::Key_F20: return gloperate::KeyF20;
            case Qt::Key_F21: return gloperate::KeyF21;
            case Qt::Key_F22: return gloperate::KeyF22;
            case Qt::Key_F23: return gloperate::KeyF23;
            case Qt::Key_F24: return gloperate::KeyF24;
            case Qt::Key_F25: return gloperate::KeyF25;
            case Qt::Key_Shift: return gloperate::KeyLeftShift;
            case Qt::Key_Control: return gloperate::KeyLeftControl;
            case Qt::Key_Alt: return gloperate::KeyLeftAlt;
            case Qt::Key_Super_L: return gloperate::KeyLeftSuper;
            case Qt::Key_AltGr: return gloperate::KeyRightAlt;
            case Qt::Key_Super_R: return gloperate::KeyRightSuper;
            case Qt::Key_Menu: return gloperate::KeyMenu;
            default: return gloperate::KeyUnknown;
        }
    }
}


/**
*  @brief
*    Constructor
*/
QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager)
: QtOpenGLWindowBase()
, m_resourceManager(resourceManager)
, m_timePropagator(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager, const QSurfaceFormat & format)
: QtOpenGLWindowBase(format)
, m_resourceManager(resourceManager)
, m_timePropagator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
QtOpenGLWindow::~QtOpenGLWindow()
{
}

/**
*  @brief
*    Get used painter
*/
Painter * QtOpenGLWindow::painter() const
{
    return m_painter;
}

/**
*  @brief
*    Set used painter
*/
void QtOpenGLWindow::setPainter(Painter * painter)
{
    // Save painter
    m_painter = painter;

    // Destroy old time propagator
    m_timePropagator.reset(nullptr);

    // Check for virtual time capability
    if (painter->supports<gloperate::AbstractVirtualTimeCapability>()) {
        // Create a time propagator that updates the virtual time
        m_timePropagator.reset(new TimePropagator(this, painter->getCapability<gloperate::AbstractVirtualTimeCapability>()));
    }
}

void QtOpenGLWindow::onInitialize()
{
    // Initialize globjects
    globjects::init();
    IF_DEBUG(globjects::DebugMessage::enable(true);)

    // Initialize painter
    if (m_painter) {
        m_painter->initialize();
    }
}

void QtOpenGLWindow::onResize(QResizeEvent * event)
{
    if (m_painter) {
        // Check if the painter supports the viewport capability
        AbstractViewportCapability * viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
        if (viewportCapability) {
            // Resize painter
            viewportCapability->setViewport(0, 0, event->size().width(), event->size().height());
        }
    }
}

void QtOpenGLWindow::onPaint()
{
    if (m_painter) {
        // Call painter
        m_painter->paint();
    }
}

void QtOpenGLWindow::keyPressEvent(QKeyEvent * event)
{
    makeCurrent();

    if (event->key() == Qt::Key_F10)
    {
        if (ScreenshotTool::isApplicableTo(painter()))
        {
            ScreenshotTool screenshot(painter(), m_resourceManager);
            screenshot.initialize();
            screenshot.save("screenshot.png");
        }
    }

    doneCurrent();

    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onKeyDown(
            fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void QtOpenGLWindow::keyReleaseEvent(QKeyEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onKeyUp(
            fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void QtOpenGLWindow::mouseMoveEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseMove(
            event->x(),
            event->y()
        );
    }
}

void QtOpenGLWindow::mousePressEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMousePress(
            event->x(),
            event->y(),
            fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::mouseReleaseEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseRelease(
            event->x(),
            event->y(),
            fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::mouseDoubleClickEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseDoubleClick(
            event->x(),
            event->y(),
            fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::wheelEvent(QWheelEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseWheel(
            event->orientation() == Qt::Vertical ? 0 : event->delta(),
            event->orientation() == Qt::Vertical ? event->delta() : 0
        );
    }
}


} // namespace gloperate-qt
