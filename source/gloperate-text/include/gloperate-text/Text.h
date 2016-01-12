
#pragma once

#include <string>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API Text
{
public:
    Text();
    virtual ~Text();

    const std::string & characters() const;
    void setCharacters(const std::string & characters);

protected:
    std::string m_data;
};


} // namespace gloperate_text
