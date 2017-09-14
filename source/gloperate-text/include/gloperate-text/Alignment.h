
#pragma once


#include <functional>

#include <cppexpose/typed/TypedEnum.h>


namespace gloperate_text
{


/**
*  @brief
*    Alignment enum class for glyphs in an annotation space.
*/
enum class Alignment : unsigned char
{
    LeftAligned, ///< Left align glyphs
    Centered,    ///< Center align glyphs
    RightAligned ///< Right align glyphs
};


} // namespace gloperate_text


namespace std
{


/**
*  @brief
*    Hash specialization for Alignment enum class.
*
*    Enables the use of Alignment as key type of the unordered collection types.
*/
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


namespace cppexpose
{


/**
*  @brief
*    Template specialization of enum strings for Alignment.
*/
template<>
class EnumValues<gloperate_text::Alignment>
{
public:
    static std::map<gloperate_text::Alignment, std::string> namedValues()
    {
        return{
            { gloperate_text::Alignment::LeftAligned, "LeftAligned" },
            { gloperate_text::Alignment::Centered, "Centered" },
            { gloperate_text::Alignment::RightAligned, "RightAligned" }
        };
    }
};


} // namespace cppexpose
