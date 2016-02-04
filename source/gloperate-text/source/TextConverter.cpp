
#include <gloperate-text/TextConverter.h>

#include <algorithm>
#include <iterator>

#include <gloperate-text/Encoding.h>


namespace gloperate_text
{


void TextConverter::encode(const std::string & input, Encoding encoding, std::u32string & output)
{
    switch (encoding)
    {
    case Encoding::ANSI:
    //case Encoding::ASCII:
        {
            output.clear();
            std::copy(input.begin(), input.end(), std::back_inserter(output));
        }
        break;
    case Encoding::Utf8:
        {
            std::size_t currentCharacterIndex = 0;
            std::size_t characterCount = 0;
            output.resize(input.size() * 4);
            while (currentCharacterIndex < input.size())
            {
                if ((input.at(currentCharacterIndex) & (1 << 7)) == 0) // 1 byte
                {
                    output.at(characterCount) = input.at(currentCharacterIndex) & 127;
                    currentCharacterIndex += 1;
                    ++characterCount;
                }
                else if ((input.at(currentCharacterIndex) & (7 << 5)) == (6 << 5)) // 2 bytes
                {
                    assert(currentCharacterIndex + 1 < input.size());
                    assert((input.at(currentCharacterIndex+1) & (2 << 6)) == (2 << 6));

                    output.at(characterCount) = (input.at(currentCharacterIndex) & 31 << 6) +
                        (input.at(currentCharacterIndex+1) & 63);
                    currentCharacterIndex += 2;
                    ++characterCount;
                }
                else if ((input.at(currentCharacterIndex) & (15 << 4)) == (14 << 4)) // 3 bytes
                {
                    assert(currentCharacterIndex + 2 < input.size());
                    assert((input.at(currentCharacterIndex+1) & (2 << 6)) == (2 << 6));
                    assert((input.at(currentCharacterIndex+2) & (2 << 6)) == (2 << 6));

                    output.at(characterCount) = (input.at(currentCharacterIndex) & 31 << 12) +
                        (input.at(currentCharacterIndex+1) & 63 << 6) +
                        (input.at(currentCharacterIndex+2) & 63);
                    currentCharacterIndex += 3;
                    ++characterCount;
                }
                else if ((input.at(currentCharacterIndex) & (31 << 3)) == (30 << 3)) // 4 bytes
                {
                    assert(currentCharacterIndex + 3 < input.size());
                    assert((input.at(currentCharacterIndex+1) & (2 << 6)) == (2 << 6));
                    assert((input.at(currentCharacterIndex+2) & (2 << 6)) == (2 << 6));
                    assert((input.at(currentCharacterIndex+3) & (2 << 6)) == (2 << 6));

                    output.at(characterCount) = (input.at(currentCharacterIndex) & 31 << 18) +
                        (input.at(currentCharacterIndex+1) & 63 << 12) +
                        (input.at(currentCharacterIndex+1) & 63 << 6) +
                        (input.at(currentCharacterIndex+2) & 63);
                    currentCharacterIndex += 4;
                    ++characterCount;
                }
                else
                {
                    assert(false);
                }
            }

            output.resize(characterCount);
        }
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

void TextConverter::encode(const char * input, size_t size, Encoding encoding, std::u32string & output)
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

void TextConverter::decode(const std::u32string & input, const char * & output, size_t size, Encoding encoding)
{
    assert(false);
}


} // namespace gloperate_text
