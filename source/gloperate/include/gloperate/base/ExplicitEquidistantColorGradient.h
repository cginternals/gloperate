
#pragma once

#include <array>
#include <vector>
#include <initializer_list>

#include <gloperate/base/Color.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/AbstractColorGradient.h>

namespace gloperate
{


class GLOPERATE_API ExplicitEquidistantColorGradient : public AbstractColorGradient
{
public:
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::vector<Color> & colors);

    template <size_t Count>
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::array<Color, Count> & colors);

    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, std::initializer_list<Color> colors);

    template <typename... Args>
    ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const Color & color, Args... args);

protected:
    std::vector<Color> m_colors;
    bool m_discrete;

protected:
    virtual Color colorAt(float position) const override;

    ExplicitEquidistantColorGradient(const std::string & name, bool discrete);
};


} // namespace gloperate

#include <gloperate/base/ExplicitEquidistantColorGradient.hpp>
