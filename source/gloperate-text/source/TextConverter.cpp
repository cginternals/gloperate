
#include <gloperate-text/TextConverter.h>

#include <algorithm>

#include <gloperate-text/Encoding.h>


namespace gloperate_text
{


void TextConverter::encode(const std::string & input, Encoding encoding, std::u32string & output)
{
    output.clear();

    switch (encoding)
    {
    case Encoding::ANSI:
    //case Encoding::ASCII:
        std::copy(input.begin(), input.end(), std::back_inserter(output));
        break;
    default:
        assert(false);
    }
}

void TextConverter::encode(const std::wstring & input, Encoding encoding, std::u32string & output)
{
    assert(false);
}

void TextConverter::encode(const std::u16string & input, Encoding encoding, std::u32string & output)
{
    assert(false);
}

void TextConverter::encode(const char * input, ssize_t size, Encoding encoding, std::u32string & output)
{
    assert(false);
}

void TextConverter::decode(const std::u32string & input, std::string & output, Encoding encoding)
{
    assert(false);
}

void TextConverter::decode(const std::u32string & input, std::wstring & output, Encoding encoding)
{
    assert(false);
}

void TextConverter::decode(const std::u32string & input, std::u16string & output, Encoding encoding)
{
    assert(false);
}

void TextConverter::decode(const std::u32string & input, const char * & output, ssize_t size, Encoding encoding)
{
    assert(false);
}


} // namespace gloperate_text
