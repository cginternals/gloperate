
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

    const std::string & string() const;
    void setString(const std::string & string);

protected:
    std::string m_string;
};


} // namespace gloperate_text
