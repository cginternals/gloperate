
#pragma once

#include <functional>

#include <reflectionzeug/property/PropertyEnum.h>


namespace gloperate_text
{

/**
*  @brief
*    The used encoding of a passed std::string.
*
*    This enum is mainly used to support the conversion of strings or a collections of characters to UTF-32, which is internally used by gloperate-text.
*/
enum class Encoding : unsigned int
{
    ANSI,
    ASCII = Encoding::ANSI, // for both ANSI and ASCII you use 1 byte in a std::string or char to encode it
    Utf8
};


} // namespace gloperate_text


namespace std
{

template<>
struct hash<gloperate_text::Encoding>
{
    std::hash<unsigned int>::result_type operator()(const gloperate_text::Encoding & arg) const
    {
        std::hash<unsigned int> hasher;
        return hasher(static_cast<unsigned int>(arg));
    }
};

} // namespace std


namespace reflectionzeug
{

template<>
struct EnumDefaultStrings<gloperate_text::Encoding>
{
    std::map<gloperate_text::Encoding, std::string> operator()()
    {
        return {
            { gloperate_text::Encoding::ASCII, "ASCII" },
            { gloperate_text::Encoding::ANSI,  "ANSI"  },
            { gloperate_text::Encoding::Utf8,  "Utf-8" }
        };
    }
};

} // namespace reflectionzeug
