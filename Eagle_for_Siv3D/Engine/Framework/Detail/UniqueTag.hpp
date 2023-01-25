#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

namespace eagle
{
	namespace backend
	{
		class UniqueTagAssetDetail;
	}

	class UniqueTagAsset;

	class UniqueTag final : Uncopyable
	{
	public:

		UniqueTag();

		UniqueTag(UniqueTag&& _other)noexcept;

		~UniqueTag();

		UniqueTag& operator=(UniqueTag&& _other)noexcept;

		void set(const String& _tag);

		const String& get()const;

		size_t code()const;

		size_t count()const;

		explicit operator bool()const noexcept;

		const String& operator*()const;

		bool operator==(const UniqueTag& _tag)const noexcept;

		bool operator==(const String& _tag)const noexcept;

		bool operator!=(const UniqueTag& _tag)const noexcept;

		bool operator!=(const String& _tag)const noexcept;

	private:

		size_t mHashCode;

		WeakObject<backend::UniqueTagAssetDetail> mAssetWeak;

		friend class UniqueTagAsset;

	};

	class UniqueTagAsset final : Uncopyable
	{
	public:

		UniqueTagAsset();

		~UniqueTagAsset();

		void secured(const String& _tag, UniqueTag& _out);

		void release(const UniqueTag& _tag);

		const String& get(size_t _hash)const;

		size_t count(const UniqueTag& _tag)const;

	private:

		SharedObject<backend::UniqueTagAssetDetail> mAssetDetail;

	};
}
