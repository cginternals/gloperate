
#pragma once


#include <glm/gtc/type_ptr.hpp>

#include <cppassist/string/conversion.h>
#include <cppassist/string/regex.h>


namespace gloperate
{


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

    if (!cppassist::string::matchesRegex(string, ss.str()))
        return false;

    std::vector<std::string> parts = cppassist::string::extract(string, elementRegex);

    assert(parts.size() == Size);

    for (unsigned i = 0; i < Size; ++i)
    {
        const std::string & part = parts[i];
        data[i] = std::is_integral<T>::value ? static_cast<T>(std::stoi(part)) : static_cast<T>(std::stod(part));
    }

    return true;
}

template <typename T>
const std::string & VectorPrefix<T>::getPrefix()
{
    static const auto prefix = std::string("");

    return prefix;
}


} // namespace gloperate


namespace cppexpose
{


template <typename BASE>
TypedFilename<BASE>::TypedFilename()
{
}

template <typename BASE>
TypedFilename<BASE>::~TypedFilename()
{
}

template <typename BASE>
bool TypedFilename<BASE>::isString() const
{
    return true;
}

template <typename BASE>
std::string TypedFilename<BASE>::toString() const
{
    return this->value().path();
}

template <typename BASE>
bool TypedFilename<BASE>::fromString(const std::string & string)
{
    this->setValue(cppassist::FilePath(string));
    return true;
}

template <typename BASE>
Variant TypedFilename<BASE>::toVariant() const
{
    return Variant::fromValue<std::string>(this->toString());
}

template <typename BASE>
bool TypedFilename<BASE>::fromVariant(const Variant & variant)
{
    return this->fromString(variant.value<std::string>());
}

template <typename BASE>
std::string TypedFilename<BASE>::typeName() const
{
    return "filename";
}


template <typename BASE>
TypedColor<BASE>::TypedColor()
{
}

template <typename BASE>
TypedColor<BASE>::~TypedColor()
{
}

template <typename BASE>
bool TypedColor<BASE>::isString() const
{
    return false;
}

template <typename BASE>
std::string TypedColor<BASE>::toString() const
{
    return this->value().toHexString();
}

template <typename BASE>
bool TypedColor<BASE>::fromString(const std::string & string)
{
    gloperate::Color color;

    if (color.fromHexString(string))
    {
        this->setValue(color);
        return true;
    }

    return false;
}

template <typename BASE>
Variant TypedColor<BASE>::toVariant() const
{
    return Variant::fromValue<std::string>(this->toString());
}

template <typename BASE>
bool TypedColor<BASE>::fromVariant(const Variant & variant)
{
    return this->fromString(variant.value<std::string>());
}

template <typename BASE>
std::string TypedColor<BASE>::typeName() const
{
    return "color";
}


template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
TypedGlmVec<VectorType, ValueType, Size, BASE>::TypedGlmVec()
{
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
TypedGlmVec<VectorType, ValueType, Size, BASE>::~TypedGlmVec()
{
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
std::string TypedGlmVec<VectorType, ValueType, Size, BASE>::toString() const
{
    return gloperate::glmToString<ValueType, Size>(glm::value_ptr(this->value()));
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
bool TypedGlmVec<VectorType, ValueType, Size, BASE>::fromString(const std::string & string)
{
    VectorType value;
    if (!gloperate::glmFromString<ValueType, Size>(string, glm::value_ptr(value)))
        return false;

    this->setValue(value);
    return true;
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
cppexpose::Variant TypedGlmVec<VectorType, ValueType, Size, BASE>::toVariant() const
{
    return toString();
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
bool TypedGlmVec<VectorType, ValueType, Size, BASE>::fromVariant(const cppexpose::Variant & value)
{
    return fromString(value.toString());
}

template <typename VectorType, typename ValueType, glm::length_t Size, typename BASE>
std::string TypedGlmVec<VectorType, ValueType, Size, BASE>::typeName() const
{
    return "glm::" + gloperate::VectorPrefix<ValueType>::getPrefix() + "vec" + cppassist::string::toString<int>(Size);
}


} // namespace cppexpose
