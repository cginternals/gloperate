
#pragma once


#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cppexpose/typed/GetTyped.h>

#include <gloperate/base/Range.h>
#include <gloperate/rendering/Color.h>


namespace gloperate
{


/**
*  @brief
*    Convert glm vector into string
*
*  @param[in] data
*    Pointer to vector data
*
*  @return
*    String representation
*/
template <typename T, glm::length_t Size>
std::string glmToString(const T * data);

/**
*  @brief
*    Convert string into glm vector
*
*  @param[in] string
*    String representation
*  @param[in] data
*    Pointer to vector data
*
*  @return
*    'true' if all went fine, else 'false'
*/
template <typename T, glm::length_t Size>
bool glmFromString(const std::string & string, T * data);

/**
*  @brief
*    Helper template to determine the vector type as string
*/
template <typename T>
class GLOPERATE_TEMPLATE_API VectorPrefix
{
public:
    static const std::string & getPrefix();
};

/**
*  @brief
*    Helper template to determine the vector type as string
*
*  @remarks
*    Spezialization for integer vector types
*/
template <>
class GLOPERATE_API VectorPrefix<int>
{
public:
    static const std::string & getPrefix();
};


} // namespace gloperate


namespace cppexpose
{


/**
*  @brief
*    Property implementation for gloperate::Color
*/
template <typename BASE>
class GLOPERATE_TEMPLATE_API TypedColor : public cppexpose::Typed<gloperate::Color, BASE>
{
public:
    TypedColor();
    virtual ~TypedColor();

    virtual std::string typeName() const override;
    bool isString() const override;

    std::string toString() const override;
    bool fromString(const std::string & string) override;

    Variant toVariant() const override;
    bool fromVariant(const Variant & variant) override;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<gloperate::Color, BASE>
{
    using Type = TypedColor<BASE>;
};


/**
*  @brief
*    Property implementation for gloperate::Range
*/
template <typename BASE>
class GLOPERATE_TEMPLATE_API TypedRange : public cppexpose::Typed<gloperate::Range, BASE>
{
public:
    TypedRange();
    virtual ~TypedRange();

    virtual std::string typeName() const override;
    bool isString() const override;

    std::string toString() const override;
    bool fromString(const std::string & string) override;

    Variant toVariant() const override;
    bool fromVariant(const Variant & variant) override;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<gloperate::Range, BASE>
{
    using Type = TypedRange<BASE>;
};


/**
*  @brief
*    Property implementation for GLM vector types
*/
template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
class GLOPERATE_TEMPLATE_API TypedGlmVec : public cppexpose::Typed<VectorType, BASE>
{
public:
    TypedGlmVec();
    virtual ~TypedGlmVec();

    virtual std::string typeName() const override;

    std::string toString() const override;
    bool fromString(const std::string & string) override;

    cppexpose::Variant toVariant() const override;
    bool fromVariant(const cppexpose::Variant & value) override;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::vec2, BASE>
{
    using Type = TypedGlmVec<glm::vec2, glm::vec2::value_type, 2, BASE>;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::ivec2, BASE>
{
    using Type = TypedGlmVec<glm::ivec2, glm::ivec2::value_type, 2, BASE>;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::vec3, BASE>
{
    using Type = TypedGlmVec<glm::vec3, glm::vec3::value_type, 3, BASE>;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::ivec3, BASE>
{
    using Type = TypedGlmVec<glm::ivec3, glm::ivec3::value_type, 3, BASE>;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::vec4, BASE>
{
    using Type = TypedGlmVec<glm::vec4, glm::vec4::value_type, 4, BASE>;
};

template <typename BASE>
struct GLOPERATE_TEMPLATE_API GetTyped<glm::ivec4, BASE>
{
    using Type = TypedGlmVec<glm::ivec4, glm::ivec4::value_type, 4, BASE>;
};


} // namespace cppexpose


#include <gloperate/base/ExtendedProperties.inl>
