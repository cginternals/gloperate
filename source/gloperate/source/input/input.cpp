#include <gloperate/input/input.h>

#include <type_traits>


namespace gloperate
{

MouseButton operator&(MouseButton lhs, MouseButton rhs)
{
    using Type = std::underlying_type<MouseButton>::type;
    Type value = static_cast<Type>(lhs) & static_cast<Type>(rhs);
    return static_cast<MouseButton>(value);
}

void operator&=(MouseButton& lhs, MouseButton rhs)
{
    lhs = lhs & rhs;
}

MouseButton operator|(MouseButton lhs, MouseButton rhs)
{
    using Type = std::underlying_type<MouseButton>::type;
    Type value = static_cast<Type>(lhs) | static_cast<Type>(rhs);
    return static_cast<MouseButton>(value);
}

void operator|=(MouseButton& lhs, MouseButton rhs)
{
    lhs = lhs | rhs;
}

MouseButton operator^(MouseButton lhs, MouseButton rhs)
{
    using Type = std::underlying_type<MouseButton>::type;
    Type value = static_cast<Type>(lhs) ^ static_cast<Type>(rhs);
    return static_cast<MouseButton>(value);
}

void operator^=(MouseButton& lhs, MouseButton rhs)
{
    lhs = lhs ^ rhs;
}

KeyModifier operator&(KeyModifier lhs, KeyModifier rhs)
{
    using Type = std::underlying_type<KeyModifier>::type;
    Type value = static_cast<Type>(lhs)& static_cast<Type>(rhs);
    return static_cast<KeyModifier>(value);
}

void operator&=(KeyModifier& lhs, KeyModifier rhs)
{
    lhs = lhs & rhs;
}

KeyModifier operator|(KeyModifier lhs, KeyModifier rhs)
{
    using Type = std::underlying_type<KeyModifier>::type;
    Type value = static_cast<Type>(lhs) | static_cast<Type>(rhs);
    return static_cast<KeyModifier>(value);
}

void operator|=(KeyModifier& lhs, KeyModifier rhs)
{
    lhs = lhs | rhs;
}

KeyModifier operator^(KeyModifier lhs, KeyModifier rhs)
{
    using Type = std::underlying_type<KeyModifier>::type;
    Type value = static_cast<Type>(lhs) ^ static_cast<Type>(rhs);
    return static_cast<KeyModifier>(value);
}

void operator^=(KeyModifier& lhs, KeyModifier rhs)
{
    lhs = lhs ^ rhs;
}

}  // namespace gloperate
