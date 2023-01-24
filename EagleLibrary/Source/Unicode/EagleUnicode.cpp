#include <Common/Unicode.hpp>

#include <nodec/unicode.hpp>

namespace eagle::Unicode
{
    
    std::u16string Utf8ToUtf16(const std::u8string& src)
    {
        return nodec::unicode::utf8to16<std::u16string>(src);
    }

    std::u32string Utf8ToUtf32(const std::u8string& src)
    {
        return nodec::unicode::utf8to32<std::u32string>(src);
    }

    std::string Utf16ToUtf8(const std::u16string& src)
    {
        return nodec::unicode::utf16to8<std::string>(src);
    }

    std::u32string Utf16ToUtf32(const std::u16string& src)
    {
        return nodec::unicode::utf16to32<std::u32string>(src);
    }

    std::string Utf32ToUtf8(const std::u32string& src)
    {
        return nodec::unicode::utf32to8<std::string>(src);
    }

    std::u16string Utf32ToUtf16(const std::u32string& src)
    {
        return nodec::unicode::utf32to16<std::u16string>(src);
    }
}