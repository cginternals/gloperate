
#pragma once

#include <functional>
#include <reflectionzeug/property/PropertyEnum.h>


namespace gloperate_text
{


enum class Alignment : unsigned char
{
    LeftAligned, Centered, RightAligned
};


} // namespace gloperate_text


namespace std
{

template<>
struct hash<gloperate_text::Alignment>
{
    std::hash<unsigned char>::result_type operator()(
        const gloperate_text::Alignment & arg) const
    {
        std::hash<unsigned char> hasher;
        return hasher(static_cast<unsigned char>(arg));
    }
};

} // namespace std

namespace reflectionzeug
{

template<>
struct EnumDefaultStrings<gloperate_text::Alignment>
{
    std::map<gloperate_text::Alignment, std::string> operator()()
    {
        return{
            { gloperate_text::Alignment::LeftAligned, "LeftAligned" },
            { gloperate_text::Alignment::Centered, "Centered" },
            { gloperate_text::Alignment::RightAligned, "RightAligned" }
        };
    }
};

} // namespace reflectionzeug
