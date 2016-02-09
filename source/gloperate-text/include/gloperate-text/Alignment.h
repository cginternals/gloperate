
#pragma once

#include <functional>
#include <reflectionzeug/property/PropertyEnum.h>


#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

enum class GLOPERATE_TEXT_API HorizontalAlignment : unsigned char
{
    Left, Center, Right
};

enum class GLOPERATE_TEXT_API VerticalAlignment : unsigned char
{
    Ascent, Center, Baseline, Descent
};


} // namespace gloperate_text


namespace std
{

template<>
struct hash<gloperate_text::HorizontalAlignment>
{
    std::hash<unsigned char>::result_type operator()(
        const gloperate_text::HorizontalAlignment & arg) const
    {
        std::hash<unsigned char> hasher;
        return hasher(static_cast<unsigned char>(arg));
    }
};

template<>
struct hash<gloperate_text::VerticalAlignment>
{
    std::hash<unsigned char>::result_type operator()(
        const gloperate_text::VerticalAlignment & arg) const
    {
        std::hash<unsigned char> hasher;
        return hasher(static_cast<unsigned char>(arg));
    }
};

} // namespace std

namespace reflectionzeug
{

template<>
struct EnumDefaultStrings<gloperate_text::HorizontalAlignment>
{
    std::map<gloperate_text::HorizontalAlignment, std::string> operator()()
    {
        return{
            { gloperate_text::HorizontalAlignment::Left,   "Left" },
            { gloperate_text::HorizontalAlignment::Center, "Center" },
            { gloperate_text::HorizontalAlignment::Right,  "Right" }
        };
    }
};

template<>
struct EnumDefaultStrings<gloperate_text::VerticalAlignment>
{
    std::map<gloperate_text::VerticalAlignment, std::string> operator()()
    {
        return{
            { gloperate_text::VerticalAlignment::Ascent,   "Ascent" },
            { gloperate_text::VerticalAlignment::Center,   "Center" },
            { gloperate_text::VerticalAlignment::Baseline, "Baseline" },
            { gloperate_text::VerticalAlignment::Descent,  "Descent" },
        };
    }
};

} // namespace reflectionzeug
