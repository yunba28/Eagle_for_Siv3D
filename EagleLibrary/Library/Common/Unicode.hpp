#pragma once

#include <string>

namespace eagle::Unicode
{

	std::u16string Utf8ToUtf16(const std::u8string& src);

	std::u32string Utf8ToUtf32(const std::u8string& src);


	std::string Utf16ToUtf8(const std::u16string& src);

	std::u32string Utf16ToUtf32(const std::u16string& src);


	std::string Utf32ToUtf8(const std::u32string& src);

	std::u16string Utf32ToUtf16(const std::u32string& src);
}