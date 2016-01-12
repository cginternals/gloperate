
#include <gloperate-text/Text.h>


namespace gloperate_text
{


Text::Text()
{
}

Text::~Text()
{
}

const std::string & Text::string() const
{
    return m_string;
}

void Text::setString(const std::string & string)
{
    m_string = string;
}


} // namespace gloperate_text
