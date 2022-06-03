
#include <gloperate-headless/SurfaceEvent.h>


namespace gloperate_headless
{


SurfaceEvent::SurfaceEvent(Type type)
: m_type(type)
, m_accepted(false)
, m_surface(nullptr)
{
}

SurfaceEvent::~SurfaceEvent()
{
}

SurfaceEvent::Type SurfaceEvent::type() const
{
    return m_type;
}

bool SurfaceEvent::isAccepted() const
{
    return m_accepted;
}

bool SurfaceEvent::isIgnored() const
{
    return !m_accepted;
}

void SurfaceEvent::setAccepted(bool accepted)
{
    m_accepted = accepted;
}

void SurfaceEvent::accept()
{
    m_accepted = true;
}

void SurfaceEvent::ignore()
{
    m_accepted = false;
}

const Surface * SurfaceEvent::surface() const
{
    return m_surface;
}

Surface * SurfaceEvent::surface()
{
    return m_surface;
}

void SurfaceEvent::setSurface(Surface * surface)
{
    m_surface = surface;
}


KeyEvent::KeyEvent(unsigned int character)
: SurfaceEvent(Type::KeyTyped)
, m_key(0)
, m_scanCode(0)
, m_action(0)
, m_modifiers(0)
, m_character(character)
{
}

KeyEvent::KeyEvent(int key, int scanCode, int action, int modifiers)
: SurfaceEvent(action == static_cast<unsigned int>(ActionType::Release) ? Type::KeyRelease : Type::KeyPress)
, m_key(key)
, m_scanCode(scanCode)
, m_action(action)
, m_modifiers(modifiers)
, m_character(0)
{
}

int KeyEvent::key() const
{
    return m_key;
}

int KeyEvent::scanCode() const
{
    return m_scanCode;
}

int KeyEvent::action() const
{
    return m_action;
}

int KeyEvent::modifiers() const
{
    return m_modifiers;
}

unsigned int KeyEvent::character() const
{
    return m_character;
}


ResizeEvent::ResizeEvent(const glm::ivec2 & size)
: SurfaceEvent(Type::Resize)
, m_size(size)
{
}

const glm::ivec2 & ResizeEvent::size() const
{
    return m_size;
}

int ResizeEvent::width() const
{
    return m_size.x;
}

int ResizeEvent::height() const
{
    return m_size.y;
}


MouseEvent::MouseEvent(const glm::ivec2 & pos)
: SurfaceEvent(Type::MouseMove)
, m_button(-1)
, m_action(-1)
, m_modifiers(0)
, m_pos(pos)
{
}

MouseEvent::MouseEvent(const glm::ivec2 & pos, const int button, const int action, const int modifiers)
: SurfaceEvent(action == static_cast<unsigned int>(ActionType::Release) ? Type::MouseRelease : Type::MousePress)
, m_button(button)
, m_action(action)
, m_modifiers(modifiers)
, m_pos(pos)
{
}

MouseEvent::MouseEvent(const glm::ivec2 & pos, const glm::vec2 & delta, int modifiers)
: SurfaceEvent(Type::Scroll)
, m_button(-1)
, m_action(-1)
, m_modifiers(modifiers)
, m_pos(pos)
, m_delta(delta)
{
}


int MouseEvent::button() const
{
    return m_button;
}

int MouseEvent::action() const
{
    return m_action;
}

int MouseEvent::modifiers() const
{
    return m_modifiers;
}

int MouseEvent::x() const
{
    return m_pos.x;
}

int MouseEvent::y() const
{
    return m_pos.y;
}

const glm::ivec2 & MouseEvent::pos() const
{
    return m_pos;
}

const glm::vec2 & MouseEvent::delta() const
{
    return m_delta;
}


PaintEvent::PaintEvent()
: SurfaceEvent(Type::Paint)
{
}


CloseEvent::CloseEvent()
: SurfaceEvent(Type::Close)
{
}


} // namespace gloperate_headless
