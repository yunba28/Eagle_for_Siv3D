#pragma once

#include <Siv3D.hpp>

namespace eagle
{
	class NameAsset;

	class Name final
	{
	public:

		Name() = default;

		Name(const Name&) = delete;

		Name& operator=(const Name&) = delete;

		Name(Name&&)noexcept = default;

		Name& operator=(Name&&)noexcept = default;

		~Name();

		void set(const s3d::String& _name);

		const s3d::String& get()const;

		const s3d::String& operator*()const;

		const size_t code()const noexcept;

		const size_t count()const;

		bool operator==(const Name& _name)const noexcept;

		bool operator==(const s3d::String& _name)const noexcept;

		bool operator!=(const Name& _name)const noexcept;

		bool operator!=(const s3d::String& _name)const noexcept;

		explicit operator bool()const noexcept;

	private:

		Name(size_t _hash, NameAsset* _ptr)noexcept;

	private:

		size_t mHashCode{ 0 };

		NameAsset* mAssetPtr{ nullptr };

		friend class NameAsset;

	};

	class NameAsset final : s3d::Uncopyable
	{
	public:

		NameAsset() = default;

		Name secured(const s3d::String& _name);

		void release(const Name& _name);

		const s3d::String& get(size_t _hash)const;

		size_t count(const Name& _name)const;

	private:

		struct Info
		{
			s3d::String name;
			s3d::uint64 base;
			s3d::int32 count;
			s3d::int32 maxCount;
		};

		s3d::HashTable<size_t, Info> mAssets;

	};
}
