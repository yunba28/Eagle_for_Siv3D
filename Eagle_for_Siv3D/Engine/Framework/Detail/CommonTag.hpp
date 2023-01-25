#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

namespace eagle
{
	namespace backend
	{
		class CommonTagAssetDetail;
	}

	class CommonTagAsset;

	class CommonTag final : Uncopyable
	{
	public:

		CommonTag();

		CommonTag(CommonTag&& _other)noexcept;

		~CommonTag();

		CommonTag& operator=(CommonTag&& _other)noexcept;

		void set(const String& _tag);

		const String& get()const;

		size_t code()const;

		size_t count()const;

		explicit operator bool()const noexcept;

		const String& operator*()const;

		bool operator==(const CommonTag& _tag)const noexcept;

		bool operator==(const String& _tag)const noexcept;

		bool operator!=(const CommonTag& _tag)const noexcept;

		bool operator!=(const String& _tag)const noexcept;

	private:

		size_t mHashCode;

		WeakObject<backend::CommonTagAssetDetail> mAssetWeak;

		friend class CommonTagAsset;

	};

	class CommonTagAsset final : Uncopyable
	{
	public:

		CommonTagAsset();

		~CommonTagAsset();

		void secured(const String& _tag, CommonTag& _out);

		void release(const CommonTag& _tag);

		const String& get(size_t _hash)const;

		size_t count(const CommonTag& _tag)const;

	private:

		SharedObject<backend::CommonTagAssetDetail> mAssetDetail;

	};
}
