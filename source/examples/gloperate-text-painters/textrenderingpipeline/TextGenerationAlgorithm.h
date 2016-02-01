
#pragma once

#include <functional>

#include <reflectionzeug/property/PropertyEnum.h>


enum class TextGenerationAlgorithm : unsigned int
{
    StaticText,
    Incrementing,
    Random
};


namespace std
{

template<>
struct hash<TextGenerationAlgorithm>
{
    std::hash<unsigned int>::result_type operator()(const TextGenerationAlgorithm & arg) const
    {
        std::hash<unsigned int> hasher;
        return hasher(static_cast<unsigned int>(arg));
    }
};

} // namespace std


namespace reflectionzeug
{

template<>
struct EnumDefaultStrings<TextGenerationAlgorithm>
{
    std::map<TextGenerationAlgorithm, std::string> operator()()
    {
        return {
            { TextGenerationAlgorithm::StaticText, "Text" },
            { TextGenerationAlgorithm::Incrementing, "Incrementing" },
            { TextGenerationAlgorithm::Random, "Random" }
        };
    }
};

} // namespace reflectionzeug
