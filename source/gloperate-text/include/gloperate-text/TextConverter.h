
#pragma once

#include <string>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


enum class Encoding : unsigned int;

class GLOPERATE_TEXT_API TextConverter
{
public:
    void encode(const std::string & input, Encoding encoding, std::u32string & output);
    void encode(const std::wstring & input, Encoding encoding, std::u32string & output);
    void encode(const std::u16string & input, Encoding encoding, std::u32string & output);
    void encode(const char * input, ssize_t size, Encoding encoding, std::u32string & output);

    void decode(const std::u32string & input, std::string & output, Encoding encoding);
    void decode(const std::u32string & input, std::wstring & output, Encoding encoding);
    void decode(const std::u32string & input, std::u16string & output, Encoding encoding);
    void decode(const std::u32string & input, const char * & output, ssize_t size, Encoding encoding);

protected:
    TextConverter();
};


} // namespace gloperate_text
