
#pragma once

#include <vector>
#include <string>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/Text.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API TextManager
{
public:
    TextManager();
    virtual ~TextManager();

    std::vector<Text> & texts();
    const std::vector<Text> & texts() const;
    const Text & text(size_t index) const;
    void setTexts(const std::vector<Text> & texts);
    void addText(const Text & text);

protected:
    std::vector<Text> m_texts;
};


} // namespace gloperate_text
