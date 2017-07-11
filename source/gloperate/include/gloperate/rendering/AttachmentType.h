
#pragma once


#include <functional>
#include <cppexpose/typed/TypedEnum.h>


namespace gloperate
{


/**
*  @brief
*    Type of a render target
*/
enum class AttachmentType : unsigned int
{
    Color,       ///< Color attachment
    Depth,       ///< Depth only attachment
    Stencil,     ///< Stencil attachment
    DepthStencil ///< Combined depth-stencil attachment
};


} // namespace gloperate


namespace std
{


/**
*  @brief
*    Hash specialization for RenderTargetType enum class.
*
*    Enables the use of RenderTargetType as key type of the unordered collection types.
*/
template<>
struct hash<gloperate::AttachmentType>
{
    std::hash<unsigned char>::result_type operator()(const gloperate::RenderTargetType & arg) const
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
*    Template specialization of enum strings for RenderTargetType.
*/
template<>
struct EnumDefaultStrings<gloperate::AttachmentType>
{
    std::map<gloperate::AttachmentType, std::string> operator()()
    {
        return {
            { gloperate::AttachmentType::Color, "Color" },
            { gloperate::AttachmentType::Depth, "Depth" },
            { gloperate::AttachmentType::Stencil, "Stencil" },
            { gloperate::AttachmentType::DepthStencil, "DepthStencil" }
        };
    }
};


} // namespace cppexpose
