#pragma once
#include <Siv3D.hpp>

namespace eagle
{
	class TagAsset;

	class Tag
	{
	public:

		Tag() = default;

		Tag(const Tag& _other);

		Tag& operator=(const Tag& _other);

		Tag(Tag&&)noexcept = default;

		Tag& operator=(Tag&&)noexcept = default;

		~Tag();

		void set(const s3d::String& _name);

		const s3d::String& get()const;

		const s3d::String& operator*()const;

		const size_t code()const noexcept;

		const size_t count()const;

		bool operator==(const Tag& _name)const noexcept;

		bool operator==(const s3d::String& _name)const noexcept;

		bool operator!=(const Tag& _name)const noexcept;

		bool operator!=(const s3d::String& _name)const noexcept;

		explicit operator bool()const noexcept;

	private:

		Tag(size_t _hash, TagAsset* _ptr)noexcept;

	private:

		size_t mHashCode{ 0 };

		TagAsset* mAssetPtr{ nullptr };

		friend class TagAsset;

	};

	class TagAsset final : s3d::Uncopyable
	{
	public:

		TagAsset() = default;

		Tag secured(const s3d::String& _name);

		void release(const Tag& _name);

		const s3d::String& get(size_t _hash)const;

		size_t count(const Tag& _name)const;

	private:

		struct Info
		{
			s3d::String name;
			s3d::int32 count;
			s3d::int32 maxCount;
		};

		s3d::HashTable<size_t, Info> mAssets;

	};
}
