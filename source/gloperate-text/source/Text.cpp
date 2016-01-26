
#include <gloperate-text/Text.h>

#include <gloperate-text/Encoding.h>


namespace gloperate_text
{


Text::Text()
{
}

Text::Text(const std::u32string & characters)
: m_data(characters)
{
}

Text::~Text()
{
}

std::u32string & Text::characters()
{
    return m_data;
}

const std::u32string & Text::characters() const
{
    return m_data;
}

void Text::setCharacters(const std::u32string & characters)
{
    m_data = characters;
}


} // namespace gloperate_text
