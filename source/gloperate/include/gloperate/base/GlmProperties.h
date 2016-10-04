
#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cppassist/string/conversion.h>
#include <cppassist/string/regex.h>

#include <cppexpose/typed/GetTyped.h>


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
std::string glmToString(const T * data)
{
    std::stringstream ss;

    ss << "(";

    for (unsigned i = 0; i < Size; ++i)
    {
        if (i > 0)
            ss << ", ";

        ss << data[i];
    }

    ss << ")";

    return ss.str();
}

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
bool glmFromString(const std::string & string, T * data)
{
    std::string elementRegex = std::is_integral<T>::value ? "(-|\\+)?\\d+" : "(-|\\+)?\\d+\\.?\\d*";

    std::stringstream ss;
    ss << "\\s*\\(";
    for (unsigned i=0; i<Size-1; ++i)
    {
        ss << "(" << elementRegex << ")";
        ss << "\\s*,\\s*";
    }
    ss << elementRegex << "\\)\\s*";

    if (!cppassist::matchesRegex(string, ss.str()))
        return false;

    std::vector<std::string> parts = cppassist::extract(string, elementRegex);

    assert(parts.size() == Size);

    for (unsigned i = 0; i < Size; ++i)
    {
        const std::string & part = parts[i];
        data[i] = std::is_integral<T>::value ? static_cast<T>(std::stoi(part)) : static_cast<T>(std::stod(part));
    }

    return true;
}

/**
*  @brief
*    Helper template to determine the vector type as string
*/
template <typename T>
class VectorPrefix
{
public:
    static std::string getPrefix()
    {
        return "";
    }
};

template <>
class VectorPrefix<int>
{
public:
    static std::string getPrefix()
    {
        return "i";
    }
};


} // namespace gloperate


namespace cppexpose
{


/**
*  @brief
*    Representation of GLM vector types
*/
template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
class TypedGlmVec : public cppexpose::Typed<VectorType, BASE>
{
public:
    TypedGlmVec()
    {
    }

    virtual ~TypedGlmVec()
    {
    }

    std::string toString() const override
    {
        return gloperate::glmToString<ValueType, Size>(glm::value_ptr(this->value())); 
    }

    bool fromString(const std::string & string) override
    {
        VectorType value;
        if (!gloperate::glmFromString<ValueType, Size>(string, glm::value_ptr(value)))
            return false;

        this->setValue(value);
        return true;
    }

    virtual std::string typeName() const override
    {
        return "glm::" + gloperate::VectorPrefix<ValueType>::getPrefix() + "vec" + cppassist::toString<int>(Size);
    }
};

template <typename BASE>
struct GetTyped<glm::vec2, BASE>
{
    using Type = TypedGlmVec<glm::vec2, glm::vec2::value_type, 2, BASE>;
};

template <typename BASE>
struct GetTyped<glm::ivec2, BASE>
{
    using Type = TypedGlmVec<glm::ivec2, glm::ivec2::value_type, 2, BASE>;
};

template <typename BASE>
struct GetTyped<glm::vec3, BASE>
{
    using Type = TypedGlmVec<glm::vec3, glm::vec3::value_type, 3, BASE>;
};

template <typename BASE>
struct GetTyped<glm::ivec3, BASE>
{
    using Type = TypedGlmVec<glm::ivec3, glm::ivec3::value_type, 3, BASE>;
};

template <typename BASE>
struct GetTyped<glm::vec4, BASE>
{
    using Type = TypedGlmVec<glm::vec4, glm::vec4::value_type, 4, BASE>;
};

template <typename BASE>
struct GetTyped<glm::ivec4, BASE>
{
    using Type = TypedGlmVec<glm::ivec4, glm::ivec4::value_type, 4, BASE>;
};


} // namespace cppexpose
