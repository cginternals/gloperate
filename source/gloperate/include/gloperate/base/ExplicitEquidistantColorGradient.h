#pragma once

#include <array>
#include <vector>
#include <initializer_list>

#include <reflectionzeug/base/Color.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/AbstractColorGradient.h>

namespace gloperate
{


class GLOPERATE_API ExplicitEquidistantColorGradient : public AbstractColorGradient
{
public:
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::vector<reflectionzeug::Color> & colors);

    template <size_t Count>
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::array<reflectionzeug::Color, Count> & colors);

    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, std::initializer_list<reflectionzeug::Color> colors);

    template <typename... Args>
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const reflectionzeug::Color & color, Args... args);

protected:
    std::vector<reflectionzeug::Color> m_colors;
    bool m_discrete;

protected:
    virtual reflectionzeug::Color colorAt(float position) const override;

    ExplicitEquidistantColorGradient(const std::string & name, bool discrete);
};


} // namespace reflectionzeug

#include <gloperate/base/ExplicitEquidistantColorGradient.hpp>
