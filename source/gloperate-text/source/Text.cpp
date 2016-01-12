
#include <gloperate-text/Text.h>


namespace gloperate_text
{


Text::Text()
{
}

Text::~Text()
{
}

const std::string & Text::characters() const
{
    return m_data;
}

void Text::setCharacters(const std::string & characters)
{
    m_data = characters;
}


} // namespace gloperate_text
