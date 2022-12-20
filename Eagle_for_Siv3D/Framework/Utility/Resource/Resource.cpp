#include "Resource.hpp"

namespace eagle
{
	void Resource::LoadTextures(const FilePath& _directory, const HashSet<String>& _includes)
	{
		for (const auto& path : FileSystem::DirectoryContents(_directory, Recursive::No))
		{
			if (FileSystem::IsDirectory(path))
			{
				LoadTextures(path, _includes);
				continue;
			}

			auto dirName = FileSystem::BaseName(_directory);
			auto fileName = FileSystem::BaseName(path);
			auto extention = FileSystem::Extension(path);

			auto assetTag = U"{}.{}"_fmt(dirName, fileName);

			if (_includes.contains(extention))
			{
				TextureAsset::Register(assetTag, path);
			}
		}
	}

	void Resource::LoadAudios(const FilePath& _directory, const HashSet<String>& _includes)
	{
		for (const auto& path : FileSystem::DirectoryContents(_directory))
		{
			if (FileSystem::IsDirectory(path))
			{
				LoadAudios(path, _includes);
				continue;
			}

			auto dirName = FileSystem::BaseName(_directory);
			auto fileName = FileSystem::BaseName(path);
			auto extention = FileSystem::Extension(path);

			if (_includes.contains(extention))
			{
				AudioAsset::Register(U"{}.{}"_fmt(dirName, fileName), path);
			}
		}
	}
}
