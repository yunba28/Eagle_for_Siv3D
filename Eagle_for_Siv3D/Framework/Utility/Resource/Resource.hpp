#pragma once

#include <Siv3D.hpp>

namespace eagle
{
	class Resource final
	{
	public:

		static void LoadTextures(
			const FilePath& _directory,
			const HashSet<String>& _includes = { U"jpg",U"png",U"jpeg",U"bmp",U"tga",U"svg" });

		static void LoadAudios(
			const FilePath& _directory,
			const HashSet<String>& _includes = { U"wav",U"mp3",U"m4a",U"ogg",U"opus",U"mid" });

	};
}
