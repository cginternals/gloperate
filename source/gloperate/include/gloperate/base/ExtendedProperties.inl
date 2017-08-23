
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
TypedColor<BASE>::TypedColor()
{
}

template <typename BASE>
TypedColor<BASE>::~TypedColor()
{
}

template <typename BASE>
std::string TypedColor<BASE>::typeName() const
{
    return "color";
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
TypedTexturePtr<BASE>::TypedTexturePtr()
{
}

template <typename BASE>
TypedTexturePtr<BASE>::~TypedTexturePtr()
{
}

template <typename BASE>
std::string TypedTexturePtr<BASE>::typeName() const
{
    return "texture";
}


template <typename BASE>
TypedRange<BASE>::TypedRange()
{
}

template <typename BASE>
TypedRange<BASE>::~TypedRange()
{
}

template <typename BASE>
std::string TypedRange<BASE>::typeName() const
{
    return "range";
}

template <typename BASE>
bool TypedRange<BASE>::isString() const
{
    return false;
}

template <typename BASE>
std::string TypedRange<BASE>::toString() const
{
    gloperate::Range range = this->value();

    float values[2] = { range.minimumValue(), range.maximumValue() };

    return gloperate::glmToString<float, 2>(values);
}

template <typename BASE>
bool TypedRange<BASE>::fromString(const std::string & string)
{
    float values[2];

    if (gloperate::glmFromString<float, 2>(string, values))
    {
        this->setValue(gloperate::Range(values[0], values[1]));
        return true;
    }

    return false;
}

template <typename BASE>
Variant TypedRange<BASE>::toVariant() const
{
    gloperate::Range range = this->value();

    Variant variant = Variant::array();
    variant.asArray()->push_back(range.minimumValue());
    variant.asArray()->push_back(range.maximumValue());

    return variant;
}

template <typename BASE>
bool TypedRange<BASE>::fromVariant(const Variant & variant)
{
    if (variant.isVariantArray())
    {
        gloperate::Range range;


        auto & vector = *variant.asArray();
        if (vector.size() >= 1) { range.setMinimumValue(vector[0].value<float>()); }
        if (vector.size() >= 2) { range.setMaximumValue(vector[1].value<float>()); }

        this->setValue(range);

        return true;
    }

    return false;
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
std::string TypedGlmVec<VectorType, ValueType, Size, BASE>::typeName() const
{
    return "glm::" + gloperate::VectorPrefix<ValueType>::getPrefix() + "vec" + cppassist::string::toString<int>(Size);
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


} // namespace cppexpose
