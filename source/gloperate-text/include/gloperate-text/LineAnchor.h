
#pragma once

#include <functional>
#include <reflectionzeug/property/PropertyEnum.h>


namespace gloperate_text
{


enum class LineAnchor : unsigned char
{
    Ascent, Center, Baseline, Descent
};


} // namespace gloperate_text


namespace std
{

template<>
struct hash<gloperate_text::LineAnchor>
{
    std::hash<unsigned char>::result_type operator()(
        const gloperate_text::LineAnchor & arg) const
    {
        std::hash<unsigned char> hasher;
        return hasher(static_cast<unsigned char>(arg));
    }
};

} // namespace std

namespace reflectionzeug
{

template<>
struct EnumDefaultStrings<gloperate_text::LineAnchor>
{
    std::map<gloperate_text::LineAnchor, std::string> operator()()
    {
        return{
            { gloperate_text::LineAnchor::Ascent, "Ascent" },
            { gloperate_text::LineAnchor::Center, "Centered" },
            { gloperate_text::LineAnchor::Baseline, "Baseline" },
            { gloperate_text::LineAnchor::Descent, "Descent" }
        };
    }
};

} // namespace reflectionzeug
