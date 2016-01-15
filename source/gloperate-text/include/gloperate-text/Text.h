
#pragma once

#include <string>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API Text
{
public:
    Text();
    Text(const std::u32string & characters);
    virtual ~Text();

    std::u32string & characters();
    const std::u32string & characters() const;
    void setCharacters(const std::u32string & characters);

protected:
    std::u32string m_data;
};


} // namespace gloperate_text
