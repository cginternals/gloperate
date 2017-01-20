
#pragma once


#include <functional>
#include <cppexpose/typed/TypedEnum.h>


namespace gloperate
{


/**
*  @brief
*    RenderTargetType enum class for identifying the valid render target
*/
enum class RenderTargetType : unsigned int
{
    Invalid,                 ///< does not contain a valid render target (yet)
    Texture,                 ///< texture
    Renderbuffer,            ///< renderbuffer
    DefaultFBOAttachment,    ///< symbolic attachment of the default renderbuffer
    UserDefinedFBOAttachment ///< user-defined renderbuffer with attachment
};


} // namespace gloperate_text


namespace std
{


/**
*  @brief
*    Hash specialization for RenderTargetType enum class.
*
*    Enables the use of RenderTargetType as key type of the unordered collection types.
*/
template<>
struct hash<gloperate::RenderTargetType>
{
    std::hash<unsigned char>::result_type operator()(
        const gloperate::RenderTargetType & arg) const
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
struct EnumDefaultStrings<gloperate::RenderTargetType>
{
    std::map<gloperate::RenderTargetType, std::string> operator()()
    {
        return{
            { gloperate::RenderTargetType::Invalid, "Invalid" },
            { gloperate::RenderTargetType::Texture, "Texture" },
            { gloperate::RenderTargetType::Renderbuffer, "Renderbuffer" },
            { gloperate::RenderTargetType::DefaultFBOAttachment, "DefaultFBOAttachment" },
            { gloperate::RenderTargetType::UserDefinedFBOAttachment, "UserDefinedFBOAttachment" }
        };
    }
};


} // namespace cppexpose
