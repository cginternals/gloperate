
#include <gloperate-text/TextManager.h>


namespace gloperate_text
{


TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

std::vector<Text> & TextManager::texts()
{
    return m_texts;
}

const std::vector<Text> & TextManager::texts() const
{
    return m_texts;
}

const Text & TextManager::text(size_t index) const
{
    return m_texts.at(index);
}

void TextManager::setTexts(const std::vector<Text> & texts)
{
    m_texts = texts;
}

void TextManager::addText(const Text & text)
{
    m_texts.push_back(text);
}


} // namespace gloperate_text
