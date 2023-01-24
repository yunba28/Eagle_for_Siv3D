#pragma once

#include <string>
#include <unordered_map>

#include <Classes/Uncopyable.hpp>
#include <Classes/SmartObject.hpp>

namespace eagle
{
	namespace Internal
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

		void set(const std::u32string& _tag);

		const std::u32string& get()const;

		size_t code()const;

		size_t count()const;

		explicit operator bool()const noexcept;

		const std::u32string& operator*()const;

		bool operator==(const UniqueTag& _tag)const noexcept;

		bool operator==(const std::u32string& _tag)const noexcept;

		bool operator!=(const UniqueTag& _tag)const noexcept;

		bool operator!=(const std::u32string& _tag)const noexcept;

	private:

		size_t mHashCode;

		WeakObject<Internal::UniqueTagAssetDetail> mAssetWeak;

		friend class UniqueTagAsset;

	};

	class UniqueTagAsset final : Uncopyable
	{
	public:

		UniqueTagAsset();

		~UniqueTagAsset();

		void secured(const std::u32string& _tag, UniqueTag& _out);

		void release(const UniqueTag& _tag);

		const std::u32string& get(size_t _hash)const;

		size_t count(const UniqueTag& _tag)const;

	private:

		SharedObject<Internal::UniqueTagAssetDetail> mAssetDetail;

	};
}

namespace eagle
{
	namespace Internal
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

		void set(const std::u32string& _tag);

		const std::u32string& get()const;

		size_t code()const;

		size_t count()const;

		explicit operator bool()const noexcept;

		const std::u32string& operator*()const;

		bool operator==(const CommonTag& _tag)const noexcept;

		bool operator==(const std::u32string& _tag)const noexcept;

		bool operator!=(const CommonTag& _tag)const noexcept;

		bool operator!=(const std::u32string& _tag)const noexcept;

	private:

		size_t mHashCode;

		WeakObject<Internal::CommonTagAssetDetail> mAssetWeak;

		friend class CommonTagAsset;

	};

	class CommonTagAsset final : Uncopyable
	{
	public:

		CommonTagAsset();

		~CommonTagAsset();

		void secured(const std::u32string& _tag, CommonTag& _out);

		void release(const CommonTag& _tag);

		const std::u32string& get(size_t _hash)const;

		size_t count(const CommonTag& _tag)const;

	private:

		SharedObject<Internal::CommonTagAssetDetail> mAssetDetail;

	};
}